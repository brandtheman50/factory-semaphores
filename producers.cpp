#include "producers.h"

void *Producers(void *arg){
    buffer * _belt = (buffer*) arg;
    int productId = _belt->producer_id;   //Assign frogBite first and increment for escargot
    ++_belt->producer_id;
    while(!sem_trywait(&_belt->goal)){  //Check if goal has not been met yet
        //Make pthread sleep before creating new candy
        if(productId == 0){
            if(_belt->frog_F)     
                usleep(_belt->frog_T * 1000);
            sem_wait(&_belt->frogBitesMax); //Check that belt contains a max of three frog bites
        }
        else if(productId == 1){
            if(_belt->everlasting_F)
                usleep(_belt->everlasting_T * 1000);
        }
        Candy *candy = new Candy(productId);
        sem_wait(&_belt->availableSlots);
        sem_wait(&_belt->mutex); //Enter critical section
        
        _belt->belt->push(candy); 
        ++_belt->OnBelt[productId]; //Increase on belt array for io
        ++_belt->Produced[productId]; //Increase produced array for io
        //Print statements
        if(productId == 0){
            io_add_type(FrogBite, _belt->OnBelt, _belt->Produced);
        }
        else if(productId == 1){
            io_add_type(Escargot, _belt->OnBelt, _belt->Produced);
        }
        sem_post(&_belt->mutex); //Exit critical section
        sem_post(&_belt->unconsumed);
    }
    return NULL;
}
