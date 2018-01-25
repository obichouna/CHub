//#define _XOPEN_SOURCE 500
//#include <ftw.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
//#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <ftw.h>
#include "networking.h"
#include "chub_headers.h"
#include "networking.h"
#define MEM_ERR 42
#define BUFFER_LENGTH 256

void process(char *s);
void subserver(int from_client);

int repo_checker_s(char * name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      return 0;
    }
  return 1;
  //checks if repo exists on the server
  //returns 0 if doesnt exist and 1 if it exists

}

int repo_checker_c(char *name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      exit(1);
    }
}

char ** chub_parse(char * line, char * arg){

  char **args = (char**)calloc(64, sizeof(char *));
  int i = 0;
  while(line){
    args[i++] = strsep(&line, arg);
  }
  args[i] = NULL;

  return args;
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

int file_send_c(char *filename, int sockfd){
  char buffer[BUFFER_SIZE];
  FILE * name = fopen(filename, "r");
  unsigned long fsize;
  if(repo_checker_c){
    printf("found file %s\n", filename);
    fseek(name, 0, SEEK_END);
    fsize = ftell(name);
    // rewind(name);

    printf("file contains %ld bytes\n", fsize);
    printf("sending file");

    while (1){
      int bytes_read = fread(buffer, sizeof(char),BUFFER_LENGTH, name);
      if (bytes_read == 0) // done reading file
	break;
      if (bytes_read < 0)
	{
      printf("problem reading from file\n");
	}
      //write will return how many bytes were written.
      // p keeps track of where in the buffer we are,
      //bytes_read to keep track of how many bytes are left to write.
      void *p = buffer;
      while (bytes_read > 0)
	{
        int bytes_written = write(sockfd, buffer, bytes_read);
        if (bytes_written <= 0)
        {
            printf("ERROR writing to socket\n");
        }
        bytes_read -= bytes_written;
        p += bytes_written;
	}
    }
    printf("file sent\n");
    return 0;
 }
 return 1;

}

int parse_s(char buffer[], int client_socket){
  // char buffer[BUFFER_LENGTH +1];
  // buffer[BUFFER_LENGTH]=0;
  char **parsed;
  // read(client_socket, buffer, BUFFER_LENGTH);
  parsed = chub_parse(buffer, " ");
  if(parsed[0]){
    
    ///for creating repo on server
    if(!strncmp("create", parsed[0], 5)){
      if(parsed[1]){
        int exists=repo_checker_s(parsed[1]);
	if(!exists){
          // printf("stuff\n");
          mkdir(parsed[1], 0666);
          printf("Created repository named: %s \n", parsed[1]);
          return 1;
	}
	printf("Repository with that name already exists. Could not create. \n");
      }
    }
    ///for cloning repo
    if(!strncmp("clone", parsed[0], 5)){
      if(parsed[1]){
        int exists=repo_checker_s(parsed[1]);
        if(exists){
          printf("stuff\n");
	        file_send_c(parsed[1], client_socket);
	        printf("Sent file named: %s \n", parsed[1]);
		// file_receive_c(parsed[1], client_socket);

          return 1;
        }else{
          printf("Repository doesn't exist. Unable to clone.\n");
        }
      }
    }
    printf("Something went wrong... please try again.\n");
    return 0;
  }
}

int main() {

  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     client_socket = server_connect(listen_socket);

     f = fork();
     if (f == 0)
       subserver(client_socket);
     else {
       subserver_count++;
       close(client_socket);
     }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      printf("[server] subserver count: %d\n", subserver_count);
    }//end stdin select
  }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (read(client_socket, buffer, sizeof(buffer))) {

    parse_s(buffer, client_socket);
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}




/*
//angelica
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    int rv = remove(fpath);
    if (rv)
        perror(fpath);
    return rv;
}
*/

    /* 
    ///for removing dir code
    if(!strncmp("remove", parsed[0], 5)){
      if(parsed[1]){
        int exists=repo_checker_s(parsed[1]);
        if(exists){
          // printf("stuff\n");
          //mdir(parsed[1], 0666);
          nftw(parsed[1], unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
          printf("Deleted repository named: %s \n", parsed[1]);
	  return 1;
        }
        printf("Repository with that name does not exist. Could not delete. \n");
      }
    }
    */
