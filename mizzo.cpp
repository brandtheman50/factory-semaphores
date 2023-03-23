#include "buffer.h"
#include "producers.h"
#include "consumers.h"

int main(int argc, char *argv[]){
    //Initialization of all variables
    buffer *conveyerBelt = new buffer;
    int arguments = 1;
    conveyerBelt->belt = new queue<Candy*>;
    conveyerBelt->ethel_F = false;
    conveyerBelt->lucy_F = false;
    conveyerBelt->frog_F = false;
    conveyerBelt->everlasting_F = false;
    conveyerBelt->ethel_T = 0;
    conveyerBelt->lucy_T = 0;
    conveyerBelt->frog_T = 0;
    conveyerBelt->everlasting_T = 0;
    conveyerBelt->producer_id = 0;
    conveyerBelt->consumed = 0;
    conveyerBelt->totals = new int*[2];
    for (int i=0; i<2; ++i){
        conveyerBelt->OnBelt[i] = 0;
        conveyerBelt->Produced[i] = 0;
        conveyerBelt->totals[i] = new int[2];
        for(int j=0; j<2; ++j){
            conveyerBelt->totals[i][j] = 0;
        }
    }
    sem_init(&conveyerBelt->mutex, 0 ,1); //Mutual buffer access
    sem_init(&conveyerBelt->unconsumed, 0, 0); //Unconsumed candies inside the belt
    sem_init(&conveyerBelt->availableSlots, 0, SLOTS); //10 slots are available in the beginning
    sem_init(&conveyerBelt->goal, 0, MAX_CANDY); //100 consumed candies is the stopping point
    sem_init(&conveyerBelt->frogBitesMax, 0, 3); //3 frog bite candies maximum allowed on the belt
    int Option;
    
    while((Option = getopt(argc, argv, "E:L:f:e:")) != -1){
        switch (Option){
            case 'E':
                conveyerBelt->ethel_F = true;
                conveyerBelt->ethel_T = atoi(optarg);
                break;
            case 'L':
                conveyerBelt->lucy_F = true;
                conveyerBelt->lucy_T = atoi(optarg);
                break;
            case 'f':
                conveyerBelt->frog_F = true;
                conveyerBelt->frog_T = atoi(optarg);
                break;
            case 'e':
                conveyerBelt->everlasting_F = true;
                conveyerBelt->everlasting_T = atoi(optarg);
                break;
            default:
                break;
        }
    }

    pthread_t frogBites;
    pthread_t escargot;
    pthread_t lucy;
    pthread_t ethel;
    
    pthread_create(&frogBites, NULL, Producers, conveyerBelt);
    pthread_create(&escargot, NULL, Producers, conveyerBelt);
    pthread_create(&lucy, NULL, Consumer, conveyerBelt);
    pthread_create(&ethel, NULL, Consumer, conveyerBelt);

    pthread_join(frogBites, NULL);
    pthread_join(escargot, NULL);
    pthread_join(lucy, NULL);
    pthread_join(ethel, NULL);
    io_production_report(conveyerBelt->Produced, conveyerBelt->totals);
}
