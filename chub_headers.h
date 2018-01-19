#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MEM_ERR 42
#define NO_ARGS 7
#define USER_EXIT 0

void chub_prompt();

char * chub_read();

char ** chub_parse(char * line, char * arg);

int chub_functions(char ** args);

void chub_initiate();

int repo_checker_s(char * name);

int repo_create_s(char * name);

int repo_delete_s(char * name);

int repo_create_c(char * name);

int repo_checker_c(char * name);

int repo_delete_c(char *name);
