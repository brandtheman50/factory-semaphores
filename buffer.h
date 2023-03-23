#ifndef BUFFER_H
#define BUFFER_H
#define SLOTS 10
#define MAX_CANDY 100

#include <queue>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <semaphore.h>
#include <string>
#include <pthread.h>
#include "production.h"

using namespace std;

class Candy {
    public:
        Candy(int id);
        int product_id;
};

struct buffer{
    sem_t mutex, unconsumed, availableSlots, goal, frogBitesMax;
    bool ethel_F, lucy_F, frog_F, everlasting_F;
    int ethel_T, lucy_T, frog_T, everlasting_T, producer_id, consumer_id, consumed;
    int OnBelt[2], Produced[2];
    int **totals;
    queue<Candy*> *belt;
};


#endif

