#include "networking.h"
#include <time.h>
//#include "chub_headers.h"
//#define MEM_ERR 42
//#define BUFFER_LENGTH 256

int file_receive_c(char *FILENAME, int sockfd);
char ** chub_parse(char * line, char * arg);
int repo_checker_c(char *name);
int parse_c(char buffer[], int server_socket);

int null_bytes(char* s){
    char * nul_pos = strchr(s,'\0');
    //checks if NULL is actually in the string
    if(nul_pos != NULL)
        return sizeof(char)*(int)(nul_pos - s);
    else
        return sizeof(char)*PACKET_SIZE;
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

int repo_checker_c(char *name){
  FILE *fs = fopen(name, "r");
  if(fs == NULL)
    {
      printf("ERROR: File %s not found.\n", name);
      return 0;
    }
  return 1;
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
	return 0;
      }
    }
    printf("Something went wrong... please try again.\n");
    return 0;
  }
  return 1;
}

int file_receive_c(char * FILENAME, int sockfd){
  char buffer[BUFFER_SIZE];
  int bytesReceived = 0;
  FILE *fp;
  // printf("stuff 1\n");
  fp = fopen(FILENAME, "r");
  // printf("stuff 2\n");
  if(NULL == fp)
    {
      printf("Error opening file\n");
      return 1;
    }
  // printf("stuff 3\n");
  /* Receive data in chunks of 256 bytes */
  while((bytesReceived = read(sockfd, buffer, 256)) > 0)
    {
      printf("stuff 4\n");
      printf("Bytes received %d\n",bytesReceived);
      // recvBuff[n] = 0;
      fwrite(buffer, 1,bytesReceived,fp);
      // printf("%s \n", recvBuff);
    }
  // printf("stuff 5\n");
  if(bytesReceived < 0)
    {
      printf("\n Read Error \n");
    }

  //printf("stuff 6\n");
  return 0;
}

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


int chub_functions(char ** args){
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

//waits for message from server
int serv_response(char * message, int server_socket){
    char buffer[BUFFER_SIZE];
    while(strcmp(buffer,message)) {
        read(server_socket, buffer, sizeof(buffer));

        //if it gets error message instead of confirmation
        if(strstr(buffer,ERROR_RESPONSE)) {
            write(server_socket, ERROR_WAIT, sizeof(ERROR_WAIT));
            read(server_socket, buffer, sizeof(buffer)); //reading follow up error message
            printf("%s",buffer);
            return -1;
        }
    }
    return 0;
}

int create (int server_socket, char * buffer ) {
  fd_set read_fds;
  while (1) {
    printf("please enter data: \n");
    fflush(stdout);
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

int chub_operations(char ** func){}

int main(int argc, char **argv) {
  int server_socket;
  char buffer[BUFFER_SIZE];
  char file[BUFFER_SIZE]; //packet size is max size of file we r sending
  char fcontent[PACKET_SIZE];
  char res[BUFFER_SIZE];
  chub_initiate();
  printf("Type the IP address of the server you want to connect to.\n");
   //char * buffer0 = calloc(1024,sizeof(char));
   memset(buffer, 0, sizeof(buffer));
   fgets(buffer, 1024, stdin);
   char * newline = strchr(buffer,'\n');
   *newline = 0;
   fd_set read_fds;
  if (argc == 2)
     server_socket = client_setup( argv[1]);
   else
     server_socket = client_setup( buffer);
    int i = 1;
    while (1) {
     printf("What would you like to do? Type 'clone' or 'create'.\n");
     //printf("Would you like to pull a file from server? Type 'clone' if so.\n");
     fgets(res,sizeof(res),stdin);
     *strchr(res, '\n') = 0;
     //clone is essentially for pulling from another computer
     if(!strcmp("clone", res)){
       //printf("in the if part");
       write(server_socket, "clone", sizeof("clone"));
       serv_response("1", server_socket);
       printf("Please enter name of file you're cloning:\n");
       fgets(file, sizeof(file), stdin);
       // *strchr('\n', file) = 0;
       write(server_socket, file, sizeof(file));
       if(!serv_response("2", server_socket)){
         memset(fcontent,0,sizeof(fcontent));
         write(server_socket, "3", sizeof("3"));//responds with a ready to read signal
         read(server_socket, fcontent, sizeof(fcontent));
         //storing file contents in client-side file
         //	printf("\nWhere would you like the file contents to be pulled?(creates new file if one doesnt exist)\n(enter a path to file): ");
	       //fgets(filePath, sizeof(filePath), stdin);
	       //	*strchr(filePath, '\n') = 0;
         int fd = open("sample.txt", O_CREAT|O_WRONLY|O_TRUNC, 0664);
         //writing into fd up to NULL
	       write(fd, fcontent, null_bytes(fcontent));
	       close(fd);
         printf("Pulled from server to client\n"); //file,filePath);
       }
       i = 0;
     }

     else if(!strcmp("exit","buffer")){
       printf("Goodbye! Come again.\n");
       close(server_socket);
       exit(0);
     }
     
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
    }



     //for creating repos on server
    if (!strncmp("create", res, 6)){

    create(server_socket, buffer);
    }
  }//end loop
}
