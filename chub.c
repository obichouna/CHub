#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "networking.h"
#include "chub_headers.h"
#define MEM_ERR 42
#define BUFFER_LENGTH 256

void chub(){
  //The main loop - Reads from stdin, parses, and runs arguments
  char * line;
  char ** args;
  int run = 1;
  //chub_initiate();
  while(run){
    //signal(SIGINT, sighandler);
    chub_prompt();
    line = chub_read();
    args = chub_parse(line, ";");
    run = chub_functions(args);
    free(line);
    free(args);
  }
}

char * chub_read(){
  //Allocates memory for reading
  char * buffer = (char *)calloc(256, sizeof(char));
  if(!buffer){
    //Checks to see if buffer really allocated
    printf("CHub: > Allocation Error\n");
    exit(MEM_ERR);
  }
  fgets(buffer, 256, stdin);
  buffer[strlen(buffer) - 1] = 0;
  return buffer;

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

void chub_prompt(){
  char cwd[256];
  getcwd(cwd, sizeof(cwd));
  fprintf(stdout, "CHub:%s$ ", cwd);
}

void chub_initiate(){
  printf(" ,-----.,--.  ,--.,--. ,--.,-----. \n");
  printf("'  .--./|  '--'  ||  | |  ||  |) /_ \n");
  printf("|  |    |  .--.  ||  | |  ||  .-.  \\ \n");
  printf("'  '--'\\|  |  |  |'  '-'  '|  '--' / \n");
  printf(" `-----'`--'  `--' `-----' `------'\n");
  printf("I didn't realize it said chub (maybe it should have been C_Hub)\n")
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
  printf("\n\nType 'login' to login to your account or 'create' to create an account \n");
  char * choice;
  while(logging_in){
    choice = chub_read();
    if(!strncmp("login", choice, 5)) {
      /*

	THIS CODE MUST BE MODIFIED TO CHECK TO MAKE SURE THE USERNAME EXISTS AND THE PASSWORD CORESPONDS

       */
      char * username;
      printf("Please type your username:\n");
      int waiting = 1;
      while(waiting){
	username = chub_read();
	if(strlen(user_name) > 0){
	  waiting = 0;
	}
      }
      printf("Please type your password:\n");
      waiting = 1;
      char * password;
       while(waiting){
	password = chub_read();
	if(strlen(password) > 0){
	  waiting = 0;
	}
      }
    }else if(!strncmp("create", choice, 6)){
      /*

	THIS CODE MUST BE MODIFIED TO CHECK IF THE USERNAME ALREADY EXISTS

       */
      char * username;
      printf("Please type your new account username:\n");
      int waiting = 1;
      while(waiting){
	username = chub_read();
	if(strlen(user_name) > 0){
	  waiting = 0;
	}
      }
      printf("Please type your password for this new account:\n");
      waiting = 1;
      char * password;
      char * confirm;
       while(waiting){
	password = chub_read();
	if(strlen(password) > 0){
	  printf("Please confirm your password:\n");
	  while(waiting){
	    confirm = chub_read;
	    if(!strcmp(password, confirm)){
	      printf("Account created\n");
	      waiting = 0;
	    }
	  }
	}
       }
    }
    logging_in = 0;
  }
}

int chub_functions(char ** args){
  if(!args[0]){
    return 0;
  }
  while(args[x]){
    char ** func = chub_parse(arg[x], " ");
    if (!strncmp("chub", func[0], 4)){
      chub_operations(func);
    }
    if (!strncmp("exit", func[0], 4)){
      exit(USER_EXIT);
    }else if (!strncmp("cd", func[0], 2)){
      chdir(func[1]);
    }else{
    int parent = fork();
    if (!parent){
      if(execvp(func[0], func) == -1){
        printf("error: incorrect input\n");
        free(func);
        exit(NO_ARGS);
      }else{
        execvp(func[0], func);
        //execvp(args[0], args);
        exit(0);
      }
    }else{
      int status;
      wait(&status);
    }
  }
    x++;
    free(func);
  }
  return 1;
}
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

int file_send_c(char *name){
  unsigned long fsize;
  if(repo_checker_c){
    printf("found file %s\n", filename);
    fseek(name, 0, SEEK_END);
    fsize = ftell(name);
    rewind(name);

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
            error("ERROR writing to socket\n");
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

int file_send_s(char *name){
   unsigned long fsize;
  if(repo_checker_c){
    printf("found file %s\n", filename);
    fseek(name, 0, SEEK_END);
    fsize = ftell(name);
    rewind(name);

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
            error("ERROR writing to socket\n");
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


int parse_s(int client_socket){
  char buffer[BUFFER_LENGTH +1];
  buffer[BUFFER_LENGTH]=0;
  char **parsed;
  read(client_socket, buffer, BUFFER_LENGTH);
  chub_parse(buffer, parsed);
  if(parsed[0]){
    if(!strncmp("create", parsed[0], 5)){
      if(parsed[1]){
	int exists=repo_checker_s(parsed[1]);
	if(!exists){
	  mkdir(parsed[1], 0666);
	  printf("Created repository named: %s \n", parsed[1]);
	  return 1;
	}
	printf("Repository with that name already exists. Could not create. \n");
      }
    }
  }
  printf("Something went wrong... please try again.\n");
  return 0;
}

//returns 1 if succeeded and 0 if failed.


int repo_checker_s(char * name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      exit(1);
    }

  //checks if repo exists on the server
  //returns 0 if doesnt exist and 1 if it exists

}


int repo_delete_s(char *name){
}
int create_c(int server_socket, char *name){
  char buffer[BUFFER_LENGTH];
  sprintf(buffer, "create %s ", name);
  write(server_socket,buffer,BUFFER_LENGTH);
  read(server_socket,buffer, 1);
  if(buffer[0]){
    fprintf(stderr, "I'm sorry, something went wrong. Failed to create %s. Please try again.\n", name);
    return 1;
  }
  return 0;
}

int repo_checker_c(char *name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      exit(1);
    }
}


int repo_delete_c(char *name){
  //deletes repo!!
}

//**** to use a chub operation you write chub <operation <repo name>*****//
int chub_operations(char ** func){
  if(func[1]){
    if(!strncmp("create", func[1], 5)){
      if(func[2] && repo_checker_s(func[2])){
	//checks if the repo exists on the server
	printf("repo already exists! choose another name or clone\n");
      }
      if(func[1]){
	repo_create_s(func[1]);
      }
      else{
	printf("Please name your repo!\n");
      }
    }
    if (!strncmp("clone", func[1], 5)){
      if(func[2] && repo_checker_s(func[2])){
      //How this will work is that the server checks if it has a repo
      //under "repo_name" and if it does, it will make a repository on the client's machine
      // if it doesnt it will return an error that the repository does not exist
      printf("Cloning %s repository into directory \n", repo_name);
      }
      else{
	printf("Error: Repo does not exist, check to make sure you're typing the correct name or create the repo\n");
      }
    }
    if(!strncmp("pull", func[1], 4)){
      //The server checks if it has a repo
      if(func[2] && repo_checker_s(func[2])){
	//delete the repo from the client
	//re create the repo on client machine"
      }
      else{
	printf("Error: Repo does not exist, check to make sure you're typing the correct name or create the repo\n");
      }
    }
    if(!strncmp("commit", func[1],6)){
      if(func[2] && repo_checker_s(func[2])){
        //checks if the repo exists on the server
      }
      else{
	printf("Error: Repo does not exist, check to make sure you're typing the correct name or create the repo\n");
      }
    }
    if(!strncmp("push", func[1],4){
      if(func[2] && repo_checker_s(func[2])){
        //checks if the repo exists on the server
	//deletes repo on server
	//recreates it based on client
      }
      else{
	printf("Error: Repo does not exist, check to make sure you're typing the correct name or create the repo\n");
      }



}


      /*int main(){

  chub_initiate();
  chub();



  }*/
