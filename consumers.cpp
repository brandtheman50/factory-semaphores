#include "consumers.h"

void *Consumer(void *arg){
    buffer * _belt = (buffer*) arg;
    Candy *candy;
    int consumer = _belt->consumer_id; //Lucy then ethel
    ++_belt->consumer_id;
    //Check that consumed is 100 or less
    while(_belt->consumed != 100){
        while(!sem_trywait(&_belt->unconsumed)){ //Check that there are unconsumed candies in belt
            //Sleep functions for pthreads
            if(consumer == 0){
                if(_belt->lucy_F)
                    usleep(_belt->lucy_T * 1000);
            }
            else if(consumer == 1){
                if(_belt->ethel_F)
                    usleep(_belt->ethel_T * 1000);
            }
            sem_wait(&_belt->mutex); //Enter critical section
            candy = _belt->belt->front();
            --_belt->OnBelt[candy->product_id];
            _belt->belt->pop(); //Remove from buffer
            
            ++_belt->totals[consumer][candy->product_id]; //Increase 2D array of total consumed for each consumer
            //Print statements from io
            if(consumer == 0){
                int lucy_C[2] = {_belt->totals[consumer][0], _belt->totals[consumer][1]}; //Assign array for io for lucy consumed
                if(candy->product_id == 0)
                    io_remove_type(Lucy, FrogBite, _belt->OnBelt, lucy_C);
                if(candy->product_id == 1)
                    io_remove_type(Lucy, Escargot, _belt->OnBelt, lucy_C);
            }
            else if(consumer == 1){
                int ethel_C[2] = {_belt->totals[consumer][0], _belt->totals[consumer][1]}; //Assign array for io for ethel consumed
                if(candy->product_id == 0)
                    io_remove_type(Ethel, FrogBite, _belt->OnBelt, ethel_C);
                if(candy->product_id == 1)
                    io_remove_type(Ethel, Escargot, _belt->OnBelt, ethel_C);
            }
            if(candy->product_id == 0)
                sem_post(&_belt->frogBitesMax);
            sem_post(&_belt->mutex); //Exit critical section
            sem_post(&_belt->availableSlots); //Signal that available slot is free
            ++_belt->consumed;
        }
    }
    return NULL;
}