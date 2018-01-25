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

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    int rv = remove(fpath);
    if (rv)
        perror(fpath);
    return rv;
}

int repo_delete_s(char *name){
  if (repo_checker_s) {
    printf("Are you sure you want to delete %s?", name);
    ans = chub_read();
    if (!strcmp(ans, "yes")){
       return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
    }
  } else {
    printf("ERROR: Repository %s not found.\n", name);
    exit(1);
  }
}
