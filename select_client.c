#include "networking.h"

int file_receive_c(char *FILENAME, int sockfd);
char ** chub_parse(char * line, char * arg);
int repo_checker_c(char *name);
int parse_c(char buffer[], int server_socket);

char ** chub_parse(char * line, char * arg){

  char **args = (char**)calloc(64, sizeof(char *));
  int i = 0;
  while(line){
    args[i++] = strsep(&line, arg);
  }
  args[i] = NULL;

  return args;
}

int repo_checker_c(char *name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      exit(1);
    }
}
int parse_c(char buffer[], int server_socket){
  // char buffer[BUFFER_LENGTH +1];
  // buffer[BUFFER_LENGTH]=0;
  char **parsed;
  char * name = "sample.txt";
  // read(client_socket, buffer, BUFFER_LENGTH);
  parsed = chub_parse(buffer, " ");
  if(parsed[0]){
    
    ///for creating repo on server
    if(!strncmp("receiving_file", parsed[0], 5)){
      if(parsed[1]){
        int exists=repo_checker_c(parsed[1]);
	if(!exists){
          // printf("stuff\n");
	  file_receive_c(name, server_socket);
          //mkdir(parsed[1], 0666);
          printf("Received file called %s \n", parsed[1]);
          return 1;
	}
	printf("Repository with that name already exists. Could not create. \n");
      }
    }
    printf("Something went wrong... please try again.\n");
    return 0;
  }
}

int file_receive_c(char *FILENAME, int sockfd){
  char buffer[BUFFER_SIZE];
  int bytesReceived = 0;
  FILE *fp;
  printf("stuff 1\n");
  fp = fopen("sample_file.txt", "ab");
  printf("stuff 2\n");
  if(NULL == fp)
    {
      printf("Error opening file");
      return 1;
    }
  printf("stuff 3\n");
  /* Receive data in chunks of 256 bytes */
  while((bytesReceived = read(sockfd, buffer, 256)) > 0)
    {
      printf("stuff 4\n");
      printf("Bytes received %d\n",bytesReceived);    
      // recvBuff[n] = 0;
      fwrite(buffer, 1,bytesReceived,fp);
      // printf("%s \n", recvBuff);
    }
  printf("stuff 5\n");
  if(bytesReceived < 0)
    {
      printf("\n Read Error \n");
    }

  printf("stuff 6\n");
  return 0;
}



int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  printf("Type the IP address of the server you want to connect to.\n");
  char * buffer0 = calloc(1024,sizeof(char));
  fgets(buffer0, 1024, stdin);
  char * newline = strchr(buffer0,'\n');
  *newline = 0;

  // int sd = client_connect(buffer0);

  /*  if (sd == -1) {
    printf("Connection refused. Try again.\n");
    exit(0);
    }*/
  
  fd_set read_fds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( buffer0);

  while (1) {

    printf("enter data: ");
    //the above printf does not have \n
    //flush the buffer to immediately print
    fflush(stdout);

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      write(server_socket, buffer, sizeof(buffer));
      read(server_socket, buffer, sizeof(buffer));
      parse_c(buffer, server_socket);
      printf("received: [%s]\n", buffer);
      //  if (strcmp(buffer,"[clone]")):
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      read(server_socket, buffer, sizeof(buffer));
      printf("[SERVER BROADCAST] [%s]\n", buffer);
      printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);
    }//end socket select

  }//end loop
}
