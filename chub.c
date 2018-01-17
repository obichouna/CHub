#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#define MEM_ERR 42

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
  //checks if repo exists on the server
}

//to use a chub operation you write chub <operation <repo name>

int chub_operations(char ** func){
  if(func[1]){
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
    if(strncmp("commit", func[1],6)){
      if(func[2] && repo_checker_s(func[2])){
        //im not really sure how this is going to work but it definitely needs to check if it exists first       
}
  
}


int main(){

  chub_initiate();
  chub();



}
