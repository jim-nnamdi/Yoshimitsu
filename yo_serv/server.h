
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

/* holds user defined data types for use ... */
struct server_handle
{
  int sockfd, newsockfd, portno, iop;
  struct sockaddr_in server_address, client_address;
  socklen_t client_data_length;
  char buffer[255];
};

void server_call_error(const char *msg);
/* holds the logic for the threaded function call */
void * thread_server_reference(void *clanx);

/* binds the server to underlying data structures */
int bind_server(struct server_handle *server_sock);

/* listens to incoming connections and set max conn */
int listen_server(struct server_handle *server_sock, int conn_count);
