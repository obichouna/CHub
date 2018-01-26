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
#define BUFFER_LEN 256

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

char ** chub_parse(char * line, char * arg){

  char **args = (char**)calloc(64, sizeof(char *));
  int i = 0;
  while(line){
    args[i++] = strsep(&line, arg);
  }
  args[i] = NULL;

  return args;
}
/* 
char * file_send_c(char *filename, int sockfd){
  char buffer[BUFFER_SIZE];
  FILE * name = fopen(filename, "w");
  unsigned long fsize;
  if(repo_checker_c){
    printf("found file %s\n", filename);
    fseek(name, 0, SEEK_END);
    fsize = ftell(name);
    rewind(name);

    printf("file contains %ld bytes\n", fsize);
    printf("sending file\n");

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
    return "receiving_file";
 }
 return 0;

}

int parse_s(char buffer[], int client_socket){
  char file[BUFFER_SIZE];
  // char buffer[BUFFER_LENGTH +1];
  // buffer[BUFFER_LENGTH]=0;
  char **parsed;
  // read(client_socket, buffer, BUFFER_LENGTH);
  parsed = chub_parse(buffer, " ");
  if(parsed[0]){
    
    ///for creating repo on server
    /*   if(!strncmp(parsed[0], "FILE", 5)){
      //printf("HI YO\n");
      write(client_socket, "1", sizeof("1")); //responds to client
      
      read(client_socket, file, sizeof(file)); //receives file name
       printf("[Server]: received [%s]\n", file);
       }*/
/* if(!strncmp("create", parsed[0], 5)){
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

*/
void chub_operation_s(){
 char buffer[BUFFER_LEN];
 char path [BUFFER_LEN];
  while (read(client_socket, buffer, sizeof(buffer))) {
    if(!strcmp(buffer, "push")){
        write(client_socket, "1", sizeof("1"));
	//handshakeeeeeee
        read(client_socket, file, sizeof(file));
	//receives file name
	printf("[server]: received [%s]\n", file);
        //file transfer code
        strcpy(path, "./server_files/");
        strcat(path,file);
        int fd;
        if((fd = open(path, O_CREAT|O_EXCL|O_WRONLY, 0664))>=0){//if file doesn't exist yet
            sprintf(perm_desc, "%s%s|%c",file,username,'\0'); //username/file already have correct endings
            int pull_perm_fd = open("./pull_perm.txt", O_CREAT|O_RDWR|O_APPEND, 0664); //to add pull permissions as well
            write(push_perm_fd, perm_desc, num_non_null_bytes(perm_desc));//writes new permission at end
            write(pull_perm_fd, perm_desc, num_non_null_bytes(perm_desc));//writes new permission at end
 
            int sd = semget(hash(file), 1, 0644 | IPC_CREAT | IPC_EXCL);//make a semaphore
            semctl(sd, 0, SETVAL, 1);//set value to 1

            write(client_socket,"2",sizeof("2"));//confirms push access
        }
        else{//file exists (push permissions need to be verified)
            read(push_perm_fd, perms_content, sizeof(perms_content));
            init_file_pos = strstr(perms_content,file); //guaranteed to be file because of ; ending
            strncpy(perm_desc,                      //copy into description of permission...
                    init_file_pos,     //starting from the beginning of description...
                    sizeof(char)*(int)(strchr(init_file_pos,'|')-init_file_pos)); //to the end of the description
            if(strstr(perm_desc,username)){//if username found in permissions
                fd = open(filePath, O_WRONLY|O_TRUNC, 0664);

		int sd = semget(hash(file), 1, 0644);
		struct sembuf temp_sembuf;
		temp_sembuf.sem_op = 1;
		temp_sembuf.sem_num = 0;
                semop(sd, &temp_sembuf, 1);//incrementing the semaphore

                write(client_socket,"2",sizeof("2"));//confirms push access
            }
            else{//push access denied
                fd = -1;
                write(client_socket,ERROR_RESPONSE,sizeof(ERROR_RESPONSE));
                wait_response(ERROR_WAIT,client_socket);
                write(client_socket,"ERROR: Push access denied\n",sizeof("ERROR: Push access denied\n"));
            }
        }
        close(push_perm_fd);

        if(fd >= 0){//if server decides to push
            //receiving file contents
            read(client_socket, fileContent, sizeof(fileContent));
            print_packet(fileContent);
            //writing into fd up to NULL
            write(fd, fileContent, num_non_null_bytes(fileContent));
            close(fd);
            printf("[Server]: pushed to '%s'\n", file);
        }
    }
    else if(!strcmp(buffer,"PULL") && !logged_in){ //dealing with a pull request
        write(client_socket, "1", sizeof("1")); //responds to client

        read(client_socket, file, sizeof(file)); //receives file name
        print_packet(file);

        //file transfer code ***
        strcpy(filePath, "./fileDir/");
        strcat(filePath,file);
        strcat(file, ";"); //adds ; to the end of file name

        int fd = -1;
        int pull_perm_fd = open("./pull_perm.txt", O_RDONLY, 0664); //to work with pull permissions
        read(pull_perm_fd, perms_content, sizeof(perms_content));
        init_file_pos = strstr(perms_content,file); //guaranteed to be file because of ; ending
        if(!init_file_pos){//if file not found
            write(client_socket,ERROR_RESPONSE,sizeof(ERROR_RESPONSE));
            wait_response(ERROR_WAIT,client_socket);
            write(client_socket,"ERROR: File not found\n",sizeof("ERROR: File not found\n"));
        }
        else{//if file found(check pull permissions)
            strncpy(perm_desc,                      //copy into description of permission...
                    init_file_pos,     //starting from the beginning of description...
                    sizeof(char)*(int)(strchr(init_file_pos,'|')-init_file_pos)); //to the end of the description
            if(strstr(perm_desc,username)){//if username found in permissions
                fd = open(filePath, O_RDONLY, 0664);

		int sd = semget(hash(file), 1, 0644);
		struct sembuf temp_sembuf;
		temp_sembuf.sem_op = -1;
		temp_sembuf.sem_num = 0;
		temp_sembuf.sem_flg = IPC_NOWAIT | SEM_UNDO;
		int temp = semop(sd, &temp_sembuf, 1);
                if (temp < 0) {//decrementing the semaphore
		  write(client_socket,ERROR_RESPONSE,sizeof(ERROR_RESPONSE));
                  wait_response(ERROR_WAIT,client_socket);
		  write(client_socket,"File currently in use\n", sizeof("File currently in use\n"));
		  fd = -1;
		}
		else{
                	write(client_socket,"2",sizeof("2"));//confirms pull access
		}
            }
            else{//pull access denied
                fd = -1;
                write(client_socket,ERROR_RESPONSE,sizeof(ERROR_RESPONSE));
                wait_response(ERROR_WAIT,client_socket);
                write(client_socket,"ERROR: Pull access denied\n",sizeof("ERROR: Pull access denied\n"));
            }
        }

        if(fd >= 0){//if server decides to pull
            //accessing file contents
            read(fd, fileContent, sizeof(fileContent));
            //sending file contents up to NULL
            wait_response("3", client_socket);
            write(client_socket, fileContent, num_non_null_bytes(fileContent));
            close(fd);
            printf("[Server]: pulled from '%s'\n", file);
        }
        close(pull_perm_fd);
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
