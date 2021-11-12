#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>


#define BufferSize 5 // Size of the buffer

sem_t empty;
sem_t full;
sem_t s;

int in = 0;
int out = 0;
char buffer[BufferSize];

void *producer(void *arg)
{   //for(int j = 0; j < MAXTIME; j++){
    while(1){
        char item = rand() % 26 + 'A'; // Produce an random item
        sem_wait(&empty);
        sem_wait(&s);
        buffer[in] = item;
        printf("Producer %d produces item %c at %d position.\n", *((int *)arg),buffer[in],in);
        in = (in+1)%BufferSize;
        sem_post(&s);
        sem_post(&full);
    }
    
    
}
void *consumer(void *cno)
{   
    //for(int j = 0; j < MAXTIME; j++){
        while(1){
        sem_wait(&full);
        sem_wait(&s);
        int item = buffer[out];
        printf("Consumer %d takes item %c from %d position.\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        sem_post(&s);
        sem_post(&empty);
    }
    

}

int main()
{   

    pthread_t pro[5],con[5];
    sem_init(&empty,0,BufferSize);// initialize semaphore
    sem_init(&full,0,0);
    sem_init(&s, 0, 1);

    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

    //create 5 producer threads and 5 consumer threads
    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&s);

    return 0;
    
}