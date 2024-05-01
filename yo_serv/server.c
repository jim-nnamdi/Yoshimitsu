
#include "server.h"
#define MAX_NO_CONN 5
#define CONN_ALIVE 1

/* handles the error message returned in event of fail*/
void server_call_error(const char *msg)
{
  perror(msg);
  exit(1);
}

int bind_server(struct server_handle *server_sock)
{
   /* following the documentation as regards socket */
  /* interaction, after the socket initialization */
  /* we need to bind the port and sockets */
  if (bind(server_sock->sockfd, (struct sockaddr *)&server_sock->server_address, sizeof(server_sock->server_address)) < 0)
    server_call_error("bind error");
  return 0;
}

int listen_server(struct server_handle *server_sock, int conn_count)
{
    /* we need to set the number of connections to */
  /* listen to: this is a basic implementation without threads */
  if (listen(server_sock->sockfd, MAX_NO_CONN) < 0)
    server_call_error("conn exceeded");
  return 0;
}

/* driver function to initialize the server
  functionalities to accept generic user input
  based on the server details ...
*/
int main(int argc, char **argv)
{
  /* check that the arguments passed are valid */
  /* we need a server name and port as str arrs */
  if (argc < 2)
  {
    fprintf(stderr, "invalid args");
    exit(1);
  }

  /* set the variables to be used in the program execution */
  /* consists of the file descriptors and socket datastructures */
  struct server_handle *server_sock = (struct server_handle*) malloc(sizeof(struct server_handle));

  /* since we're building on top of TCP we use sock_stream */
  server_sock->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock->sockfd < 0)
  {
    fprintf(stderr, "conn error");
    exit(1);
  }

  /* set the port number received from the user */
  /* convert the string value to an int value */
  server_sock->portno = atoi(argv[1]);

  /* clear the server address buffer and specify */
  /* the various parameters set in the server addr */
  bzero((char *)&server_sock->server_address, sizeof(server_sock->server_address));
  server_sock->server_address.sin_family = AF_INET;
  server_sock->server_address.sin_addr.s_addr = INADDR_ANY;
  server_sock->server_address.sin_port = htons(server_sock->portno);

  /* following the documentation as regards socket */
  /* interaction, after the socket initialization */
  /* we need to bind the port and sockets */
  if (bind_server(server_sock) < 0)
    server_call_error("bind error");

  /* we need to set the number of connections to */
  /* listen to: this is a basic implementation without threads */
  if (listen_server(server_sock, MAX_NO_CONN) < 0)
    server_call_error("conn exceeded");

  /* after the bind and listen operations we need */
  /* to accept the incoming connection from the clients */
  /* we might have multiple incoming clients requests */
  server_sock->client_data_length = sizeof(server_sock->client_address);
  server_sock->newsockfd = accept(server_sock->sockfd, (struct sockaddr *)&server_sock->client_address, &server_sock->client_data_length);
  if (server_sock->newsockfd < 0)
  {
    fprintf(stderr, "accept error");
    exit(1);
  }

  /* we want to keep the connection alive until */
  /* until an underlying protocol ends the connection based on set rules */
  while (CONN_ALIVE)
  {
    /* clear the buffer which stores and holds the data */
    /* which comes in after the client connects to the server */
    bzero(server_sock->buffer, 255);
    server_sock->iop = read(server_sock->newsockfd, server_sock->buffer, 255);
    if (server_sock->iop < 0)
      server_call_error("read error");

    bzero(server_sock->buffer, 255);
    fgets(server_sock->buffer, 255, stdin);
    /* after clearing the buffer and taking data */
    /* from the standard input, we then write response */
    server_sock->iop = write(server_sock->newsockfd, server_sock->buffer, 255);
    if (server_sock->iop < 0)
      server_call_error("write error");

    /* specify a protocol to terminate server connection */
    if (strncmp(server_sock->buffer, "bye", 3) == 0)
      break;
  }

  /* close the connections to the accept & socket fds */
  close(server_sock->newsockfd);
  close(server_sock->sockfd);
}