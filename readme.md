# Yoshimitsu
MultiThreaded version of a custom protocol built on TCP/IP stack. here am implementing a more rather generic protocol to control the messages sent over in this network, apparently the main use case is for a centralized IRC Network. the code flow follows basic server-client implementations, where the server binds and accepts connections from the clients and the clients connects and sends messages to the server and vice versa.

# Modules
- [x] server module : holds the implementation for the server codes
- [x] client module : holds the implementation for the client codes
- [x] rough thread patch : holds a rough thread implementation (check sample below : ref [yo_thread/y_thread.c])

```shell 

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

```
# Todos
- [x] At what point should mutexes be introduced into the sockets
- [x] How do we create threads for each individual connections

# Notes
This project is still work in progress