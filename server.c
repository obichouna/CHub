#include <sys/socket.h>
#include <sys/types.h>
#include<netdb.h>
#define BUFFER_LENGTH 256


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

int sd; 
sd = socket(AD_INET, SOCK_STREAM, 0);

struct addrinfo * hints, * results;
hints = (struct addrinfo*) calloc(1,sizeof(struct(struct addrinfo));
hints -> ai_faimly = AF_INET;
hints-> ai_socktupe = SOCK_STREAM //TCP socket
hints -> ai_flags =AI_PASSIVE // only needed on server
getaddrinfo(NULL, “80”, hints, &results); //Server sets node to NULL
				  
listen(sd, 10);
int client_socket; 
socklen_t sock_size;
struct sockaddr_storage client_address; 
	
client_socket = accept(sd,(struct sockaddr*)&client_address, &soc_size);
				  
free(hints);
freeaddrinfo(results);
