#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

pthread_t listen_thread;
int is_listening = 0;
int no_connections = 0;

pthread_mutex_t tmutex = PTHREAD_MUTEX_INITIALIZER;

struct process_threads
{
  void* (*TCPServerListen_ThreadProcess) (void*);
  int (*Listen_ThreadProcess) ();
};

int add()
{
  printf("%s\n", "hello worked!");
  return 0;
}

void * TCPServerListen_ThreadProcess(void* arg)
{
  struct process_threads px;
  is_listening = 1;
  px.Listen_ThreadProcess = &add;
  px.Listen_ThreadProcess();
  return NULL;
}

void listen_server()
{
  pthread_create(&listen_thread, NULL, &TCPServerListen_ThreadProcess, listen_server);
  pthread_join(listen_thread, NULL);
}

int main()
{
  listen_server();
  return 0;
}