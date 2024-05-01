
#include "server.h"
#define MAX_NO_CONN 5
#define CONN_ALIVE 1

/* handles the error message returned in event of fail*/
void server_call_error(const char *msg) 
{
  perror(msg);
  exit(1);
}

/* driver function to initialize the server 
  functionalities to accept generic user input
  based on the server details ...
*/
int main(int argc, char **argv) 
{
  /* check that the arguments passed are valid */
  /* we need a server name and port as str arrs */
  if (argc < 2) {
    fprintf(stderr, "invalid args");
    exit(1);
  }

  /* set the variables to be used in the program execution */
  /* consists of the file descriptors and socket datastructures */
  int sockfd, newsockfd, portno, iop;
  struct sockaddr_in server_address, client_address;
  socklen_t client_data_length;
  char buffer[255];

  /* since we're building on top of TCP we use sock_stream */
  sockfd = socket(AF_INET, SOCK_STREAM, 0 );
  if (sockfd < 0 )
  {
    fprintf(stderr, "conn error");
    exit(1);
  }

  /* set the port number received from the user */
  /* convert the string value to an int value */
  portno = atoi(argv[1]);

  /* clear the server address buffer and specify */
  /* the various parameters set in the server addr */
  bzero((char*)&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(portno);

  /* following the documentation as regards socket */
  /* interaction, after the socket initialization */
  /* we need to bind the port and sockets */
 if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0 )
    server_call_error("bind error");

  /* we need to set the number of connections to */
  /* listen to: this is a basic implementation without threads */
  if (listen(sockfd, MAX_NO_CONN) < 0 )
      server_call_error("conn exceeded");
  
  /* after the bind and listen operations we need */
  /* to accept the incoming connection from the clients */
  /* we might have multiple incoming clients requests */
  client_data_length = sizeof(client_address);
  newsockfd = accept(sockfd, (struct sockaddr*)&client_address, &client_data_length);
  if (newsockfd < 0 ){
    fprintf(stderr, "accept error");
    exit(1);
  }

  /* we want to keep the connection alive until */
  /* until an underlying protocol ends the connection based on set rules */
  while(CONN_ALIVE) {
    /* clear the buffer which stores and holds the data */
    /* which comes in after the client connects to the server */
    bzero(buffer, 255);
    iop = read(newsockfd, buffer, 255);
    if (iop <  0) 
        server_call_error("read error");
    
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    /* after clearing the buffer and taking data */
    /* from the standard input, we then write response */
    iop = write(newsockfd, buffer, 255);
    if (iop < 0)
        server_call_error("write error");
    
    /* specify a protocol to terminate server connection */
    if (strncmp(buffer, "bye", 3) == 0)
      break;
  }

  /* close the connections to the accept & socket fds */
  close(newsockfd);
  close(sockfd);
}