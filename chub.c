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
  printf(" ,-----.,--.  ,--.,--. ,--.,-----. \n");
  printf("'  .--./|  '--'  ||  | |  ||  |) /_ \n");
  printf("|  |    |  .--.  ||  | |  ||  .-.  \\ \n");
  printf("'  '--'\\|  |  |  |'  '-'  '|  '--' / \n");
  printf(" `-----'`--'  `--' `-----' `------'\n");
  printf("I didn't realize it said chub (maybe it should have been C_Hub)\n")
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
  printf("Type 'login' to login to your account or 'create' to create an account \n");
  char * choice;
  while(logging_in){
    choice = chub_read();
    if(!strncmp())
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

int chub_operations(char ** func){
  if (!strncmp("clone", func[1], 5)){
    //How this will work is that the server checks if it has a repo
    //under "repo_name" and if it does, it will make a repository on the client's machine
    printf("Cloning %s repository into directory \n", repo_name);
  }
}


int main(){

  chub_initiate();
  chub();



}
