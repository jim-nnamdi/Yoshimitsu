
#include "client.h"
#define CONN_ALIVE 1

struct client_handle {
  int sockfd;                               /* file descriptor for socket connection */
  int portno;                               /* holds the port number for the client */
  int iopx;                                 /* for the read and writes operations */
  struct sockaddr_in server_address;        /* socket address struct for server */
  struct hostent *server;                   /* hosts data coming from the server */
  char buffer[255];                         /* holds the data sent to and fro */
};

/* handle the errors that occur in the client side */
/* returns from standard error for user */
void client_call_error(const char *msg)
{
  perror(msg);
  exit(1);
}


/* main function handles the main driver code for */
/* client connections and interactions for the client */
int main(int argc, char **argv)
{
  /* check that the number of arguments passed are valid */
  /* we need the client binary . server base port . client port */
  if (argc < 3) 
  {
    fprintf(stderr, "invalid args");
    exit(1);
  }

  /* specify the data types to be implemented for the client */
  /* this includes the file descriptors and handles for ports */
  struct client_handle *client_sock = (struct client_handle *) malloc(sizeof(struct client_handle));

  /* initialize the base socket connection and specify sock_stream */
  /* we're building on TCP connections here [same as server impl] */
  client_sock->sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (client_sock->sockfd < 0) 
  {
    fprintf(stderr, "conn error");
    exit(1);
  }

  /* clear the server address buffer and also set the port no */
  /* convert from string to int and set to host to network short */
  client_sock->portno = atoi(argv[2]);
  bzero((char*)&client_sock->server_address, sizeof(client_sock->server_address));
  client_sock->server_address.sin_family = AF_INET;

  /* copy the data from the server to the server address */
  bcopy((char*)&client_sock->server->h_addr, (char*)&client_sock->server_address.sin_addr, client_sock->server->h_length);
  client_sock->server_address.sin_addr.s_addr = INADDR_ANY;
  client_sock->server_address.sin_port = htons(client_sock->portno);

  /* for the client, after the socket connection has been set */
  /* we then connect to the server directly */
  if (connect(client_sock->sockfd, (struct sockaddr*)&client_sock->server_address, sizeof(client_sock->server_address)) < 0 )
      client_call_error("connection error");

  /* we need to keep the client connection alive because */
  /* we always want to get the write responses from the server */
  /* until the connection to the server is terminated */
  while(CONN_ALIVE)
  {
    /* clear the data buffer for the user data from client */
    bzero(client_sock->buffer, 255);
    fgets(client_sock->buffer, 255, stdin);
    
    /* write data from the client to the server */
    client_sock->iopx = write(client_sock->sockfd, client_sock->buffer, 255);
    if (client_sock->iopx < 0)
      client_call_error("write error");
    
    /* read whatever the server sends back to client */
    client_sock->iopx = read(client_sock->sockfd, client_sock->buffer, 255);
    if (client_sock->iopx < 0)
      client_call_error("read error");

    /* write a simple rule to close the connection */
    if (strncmp(client_sock->buffer, "bye", 3) == 0)
        break;
  }
  /* close the connection to the socket server desc */
  close (client_sock->sockfd);
}