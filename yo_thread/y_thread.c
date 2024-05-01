
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

void * sample_socket_thread(void* argv) 
{
  int sockfd, newsockfd, portno, io;
  struct sockaddr_in sv_addr, cv_addr;
  char buffer[255];
  socklen_t cv_len;
  char** arrg;
  pthread_mutex_lock(&threaded_mutex);        /* lock the operation here ... */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) perror("conn error");
  arrg = (char**) argv;
  portno = atoi(arrg[1]);
  bzero((char*)&sv_addr, sizeof(sv_addr));
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_addr.s_addr = INADDR_ANY;
  sv_addr.sin_port = htons(portno);
  if(bind(sockfd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) < 0) perror("bind error");
  if(listen(sockfd, 5) < 0) perror("listen error");
  cv_len = sizeof(cv_addr);
  newsockfd = accept(sockfd,(struct sockaddr*)&cv_addr, &cv_len);
  if(newsockfd < 0) perror("accept error");
  while(1)
  {
    bzero(buffer, 255);
    io = read(newsockfd, buffer, 255);
    if(io < 0) perror("read error");
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    io = write(newsockfd, buffer, 255);
    if(io < 0) perror("write error");
    if(strncmp(buffer, "bye", 3) == 0) break;
  }
  pthread_mutex_unlock(&threaded_mutex);       /* unlock to free memory space */
  close(newsockfd);
  close(sockfd);
  return NULL;
} 

int sample_main(int argc, char** argv)
{
  if(argc < 2) perror("invalid args");
  pthread_mutex_init(&threaded_mutex, NULL);
  pthread_t sample_tid0;
  pthread_create(&sample_tid0,NULL, &sample_socket_thread, &argc);
  pthread_join(sample_tid0, NULL);
  pthread_mutex_destroy(&threaded_mutex);
  return 0;
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