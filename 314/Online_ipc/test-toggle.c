#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10

zem_t zem[NUM_THREADS];

void *justprint(void *data)
{
  int thread_id = *((int *)data);

  while(1)
    {
      zem_down(&zem[thread_id]);
      printf("%c\n", thread_id+65);
      zem_up(&zem[(thread_id+1)%NUM_THREADS]);
      if(thread_id==2){
        printf("%c\n", 88);
      }
    }
  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++){
    zem_init(&zem[i], 0);
  }

  zem_up(&zem[0]);

  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}