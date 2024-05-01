
#include "client.h"
#define CONN_ALIVE 1

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
  int sockfd, portno, cio;
  struct sockaddr_in server_address;
  struct hostent *server;
  char buffer[255];

  /* initialize the base socket connection and specify sock_stream */
  /* we're building on TCP connections here [same as server impl] */
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sockfd < 0) 
  {
    fprintf(stderr, "conn error");
    exit(1);
  }

  /* clear the server address buffer and also set the port no */
  /* convert from string to int and set to host to network short */
  portno = atoi(argv[2]);
  bzero((char*)&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;

  /* copy the data from the server to the server address */
  bcopy((char*)&server->h_addr, (char*)&server_address.sin_addr, server->h_length);
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(portno);

  /* for the client, after the socket connection has been set */
  /* we then connect to the server directly */
  if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0 )
      client_call_error("connection error");

  /* we need to keep the client connection alive because */
  /* we always want to get the write responses from the server */
  /* until the connection to the server is terminated */
  while(CONN_ALIVE)
  {
    /* clear the data buffer for the user data from client */
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    
    /* write data from the client to the server */
    cio = write(sockfd, buffer, 255);
    if (cio < 0)
      client_call_error("write error");
    
    /* read whatever the server sends back to client */
    cio = read(sockfd, buffer, 255);
    if (cio < 0)
      client_call_error("read error");

    /* write a simple rule to close the connection */
    if (strncmp(buffer, "bye", 3) == 0)
        break;
  }
  /* close the connection to the socket server desc */
  close (sockfd);
}