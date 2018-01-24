#include "networking.h"
#include "chub_headers.h"
#define MEM_ERR 42
#define BUFFER_LENGTH 256
/**
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
**/
//creating repo

char * ask_for_dir(){
  printf("Please type the directory name you would like to use and press enter: ");
  char * buffer = calloc(1024,sizeof(char));
  fgets(buffer, 1024, stdin);
  char * newline = strchr(buffer,'\n');
  *newline = 0;
  return buffer;
}

int create_dir(char *dir_name) {
  int status = 0;
  return status;
} 

int main(int argc, char **argv) {
  int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  // while (1) {
    /* char * dir_name = ask_for_dir();
    // *strchr(dir_name, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
    // create_dir(ask_for_dir());
    //printf("enter directory name: ");
    }*/

  while (1) {
    printf("enter directory name: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
    }
}
