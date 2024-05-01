
/* this is a rough sample patch of what the threaded */
/* version of the network protocol calls would look like */
/* not to be used in production */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#define TRUE 1


/* to avoid segmentation faults and data races */
/* we initialize some mutual exclusion variables */
pthread_mutex_t threaded_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t threaded_condv  = PTHREAD_COND_INITIALIZER;
int mutex_data_set = 0; 

/* threads always take a function as an argument */
/* these functions have constraints such that the */
/* function has to return a void pointer and also */
/* take a void pointer as an argument too */
void * threaded_reference()
{
  pthread_mutex_lock(&threaded_mutex);    
  int value = ((rand() % 7) + 1);
  int* res = malloc(sizeof(int));
  *res = value;
  mutex_data_set = 1;                     /* data has been set by rand value call */
  pthread_cond_signal(&threaded_condv);   /* signal sent to the conditional variable */
  printf("threaded_mem_add: %p\t threaded_mem_value:%d\n", res, *res);
  pthread_mutex_unlock(&threaded_mutex);  /* unlock the mutex and release CPU mem space */
  return (void *)res;
}

int main()
{
  /* let this variable hold the data returned */
  /* from the threaded reference func call */
  pthread_mutex_init(&threaded_mutex, NULL);
  int* res;
  srand(time(NULL));

  /* create the threads locally scoped */
  pthread_t ref_tidx;
  pthread_t ref_tidy;

  /* lock the thread for data processing */
  /* create the threads implementations */
  pthread_create(&ref_tidx, NULL, &threaded_reference, NULL);
  pthread_create(&ref_tidy, NULL, &threaded_reference, NULL);

  /* make the threads joinable*/
  pthread_join(ref_tidx, (void **) &res);
  pthread_join(ref_tidy, (void **) &res);
  printf("main_mem: %p\t main_mem_value:%d\n", res, *res);
  free(res);
  /* you can also use the pthread_exit to hold the main thread execution */
  /* until the threaded_reference threads complete execution */
  // pthread_exit(NULL);
  pthread_mutex_destroy(&threaded_mutex);
  return 0;
}