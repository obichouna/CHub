#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>
/* basic server file stuff to make fit so it works with our code */
/*Receive File from Client */
char* fr_name = "/home/aryan/Desktop/receive.txt";
FILE *fr = fopen(fr_name, "a");
if(fr == NULL)
  printf("File %s Cannot be opened file on server.\n", fr_name);
 else
   {
     bzero(revbuf, LENGTH); 
     int fr_block_sz = 0;
     while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
       {
	 int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
	 if(write_sz < fr_block_sz)
	   {
	     error("File write failed on server.\n");
	   }
	 bzero(revbuf, LENGTH);
	 if (fr_block_sz == 0 || fr_block_sz != 512) 
	   {
	     break;
	   }
       }
     if(fr_block_sz < 0)
       {
	 if (errno == EAGAIN)
	   {
	     printf("recv() timed out.\n");
	   }
	 else
	   {
	     fprintf(stderr, "recv() failed due to errno = %d\n", errno);
	     exit(1);
	   }
       }
     printf("Ok received from client!\n");
     fclose(fr); 
   }
/* Send File to Client */
//if(!fork())
//{
char* fs_name = "/home/aryan/Desktop/output.txt";
char sdbuf[LENGTH]; // Send buffer
printf("[Server] Sending %s to the Client...", fs_name);
FILE *fs = fopen(fs_name, "r");
if(fs == NULL)
  {
    fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
    exit(1);
  }

bzero(sdbuf, LENGTH); 
int fs_block_sz; 
while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0)
  {
    if(send(nsockfd, sdbuf, fs_block_sz, 0) < 0)
      {
	fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
	exit(1);
      }
    bzero(sdbuf, LENGTH);
  }
printf("Ok sent to client!\n");
success = 1;
close(nsockfd);
printf("[Server] Connection with Client closed. Server will wait now...\n");
while(waitpid(-1, NULL, WNOHANG) > 0);
//}
}
