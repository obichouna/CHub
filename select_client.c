#include "networking.h"
//#include "chub_headers.h"
//#define MEM_ERR 42
#define BUFFER_LEN 256
#define FILE_LEN 32768
int file_receive_c(char *FILENAME, int sockfd);
char ** chub_parse(char * line, char * arg);
int repo_checker_c(char *name);
int parse_c(char buffer[], int server_socket);
/*
char ** chub_parse(char * line, char * arg){

  char **args = (char**)calloc(64, sizeof(char *));
  int i = 0;
  while(line){
    args[i++] = strsep(&line, arg);
  }
  args[i] = NULL;

  return args;
}
*/ 
int repo_checker_c(char *name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      exit(1);
    }
}
/*
int parse_c(char buffer[], int server_socket){
  // char buffer[BUFFER_LENGTH +1];
  // buffer[BUFFER_LENGTH]=0;
  char **parsed;
  char * name = "sample.txt";
  // read(client_socket, buffer, BUFFER_LENGTH);
  parsed = chub_parse(buffer, " ");
  if(parsed[0]){

    ///for creating repo on server
    if(!strncmp("clone", parsed[0], 5)){
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
*/
/*
int file_receive_c(char * FILENAME, int sockfd){
  char buffer[BUFFER_SIZE];
  int bytesReceived = 0;
  FILE *fp;
  printf("stuff 1\n");
  fp = fopen(FILENAME, "r");
  printf("stuff 2\n");
  if(NULL == fp)
    {
      printf("Error opening file");
      return 1;
    }
  printf("stuff 3\n");
  /* Receive data in chunks of 256 bytes */
/* while((bytesReceived = read(sockfd, buffer, 256)) > 0)
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
*/ 

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
  printf("I didn't realize it said chub (maybe it should have been C_Hub)\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
  printf("\n\nType 'login' to login to your account or 'create' to create an account \n");
  char * choice;
  int logging_in = 1;
  while(logging_in){
    choice = chub_read();
    if(!strncmp("login", choice, 5)) {


	//THIS CODE MUST BE MODIFIED TO CHECK TO MAKE SURE THE USERNAME EXISTS AND THE PASSWORD CORESPONDS


      char * username;
      printf("Please type your username:\n");
      int waiting = 1;
      while(waiting){
	username = chub_read();
	if(strlen(username) > 0){
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


	//THIS CODE MUST BE MODIFIED TO CHECK IF THE USERNAME ALREADY EXISTS


      char * username;
      printf("Please type your new account username:\n");
      int waiting = 1;
      while(waiting){
	username = chub_read();
	if(strlen(username) > 0){
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


/* int chub_functions(char ** args){
  if(!args[0]){
    return 0;
  }
  int x = 0;
  while(args[x]){
    char ** func = chub_parse(args[x], " ");
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

*/

int wait_c(char * message, int server_socket){
    char buffer[BUFFER_LEN];
    while(strcmp(buffer,message)) {
        read(server_socket, buffer, sizeof(buffer));

        //if it gets error message instead of confirmation
        if(strncmp(buffer,"error", 5)) {
            printf("Something went wrong... Please try again.\n");
            return -1;
        }
    }
    return 0;
}
//waits until it receive message from server
//returns -1 if error
//returns 0 if worked

void which_file(int server_socket){
   printf("\n Please type the name of file:: ");
   fgets(file, sizeof(file), stdin);
   *strchr(file, '\n') = 0;
   write(server_socket, file, sizeof(file));
}
int chub_operations(char * func, int server_socket){
  char fileStuff[FILE_LEN]; 
  if(!strcmp("pull",func)){
   //first: tell it to pull
    write(server_socket, "pull", 4)); //pull request sent
    wait_c("1", server_socket);
    //second: tell it file
    which_file(server_socket);
    if(!wait_c("2", server_socket)){
      //receiving file contents
      write(server_socket, "3", 1);
      read(server_socket, fileStuff, sizeof(fileStuff));
                //storing file contents in client-side file
                printf("\nWhere would you like the file contents to be pulled?(creates new file if one doesnt exist)\n(enter a path to file): ");
                fgets(filePath, sizeof(filePath), stdin);
                *strchr(filePath, '\n') = 0;
                int fd = open(filePath, O_CREAT|O_WRONLY|O_TRUNC, 0664);
                //writing into fd up to NULL
                write(fd, fileContent, num_non_null_bytes(fileContent));
                close(fd);

                printf("Pulled from '%s' to '%s'\n", file,filePath);
            }

        }

else if(!strcmp("exit",buffer)) {
            printf("Thank you for using FTP. Goodbye\n");
            close(server_socket);
            exit(0);
        }

!strcmp("push",buffer)){
            //telling server push
            write(server_socket, "push", 4); //push request sent
            wait_response("1", server_socket);

            //sending file name
            printf("\n What is the name of the file you are pushing into?(if it doesnt exist yet one will be created): ");
            fgets(file, sizeof(file), stdin);
            *strchr(file, '\n') = 0;
            write(server_socket, file, sizeof(file)); //file name sent

            if(!wait_response("2", server_socket)){//wait for confirmation to send file contents
                //file transfer
                printf("\nWhat is the path to this file?: ");
                fgets(filePath, sizeof(filePath), stdin);
                *strchr(filePath, '\n') = 0;
                //accessing file contents
                int fd;
                if ((fd = open(filePath, O_RDONLY)) < 0) //checks if file exists
                    handle_error();
                read(fd, fileContent, sizeof(fileContent));
                close(fd);
                //sending file contents up to NULL
                write(server_socket, fileContent, num_non_null_bytes(fileContent));
                printf("Pushed from '%s' to '%s'\n", filePath, file); //***
            }

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  chub_initiate();
  //chub();

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
    printf("please enter data: \n");
    //  printf("Would you like to get a file from server?\n");
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
      //parse_c(buffer, server_socket);
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
      parse_c(buffer, server_socket);
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);
    }//end socket select

  }//end loop
}
