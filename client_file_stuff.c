
/* Send File to Server */
char* fs_name = "/home/aryan/Desktop/quotidiani.txt";
char sdbuf[LENGTH]; 
printf("[Client] Sending %s to the Server... ", fs_name);
FILE *fs = fopen(fs_name, "r");
if(fs == NULL)
  {
    printf("ERROR: File %s not found.\n", fs_name);
    exit(1);
  }

bzero(sdbuf, LENGTH); 
int fs_block_sz; 
while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
  {
    if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
      {
	fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
	break;
      }
    bzero(sdbuf, LENGTH);
  }
printf("Ok File %s from Client was Sent!\n", fs_name);


/* Receive File from Server */
    printf("[Client] Receiveing file from Server and saving it as final.txt...");
    char* fr_name = "/home/aryan/Desktop/progetto/final.txt";
    FILE *fr = fopen(fr_name, "a");
    if(fr == NULL)
        printf("File %s Cannot be opened.\n", fr_name);
    else
    {
        bzero(revbuf, LENGTH); 
        int fr_block_sz = 0;
        while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
        {
            int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
            if(write_sz < fr_block_sz)
            {
                error("File write failed.\n");
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
            }
        }
        printf("Ok received from server!\n");
        fclose(fr);
    }
