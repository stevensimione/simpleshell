#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include "debug.h"
#include "sfish.h"

extern char* prevLocation;
extern sigset_t mask;
extern pid_t pid;

int countChar(char* string, char c);
char* splitString(char* string, char c);
char* newArgs(char* ptr[], char* buffer, int num);
int containsSlash(char* string);

void help();
char* getdir(int len);
char* removeWhiteSpace(char* input);
void ischdir(char* input);
int withWhite(char* input, int len);

int redirectOrPipe(char* input, char* envp[]);

int containsPartThree(char* input);

int myExec(char* input, char* envp[]);
void handler(int sig);
char* trimString(char* input);

/*
    FINISH COLOR

    ALSO make sure to not leave space between pipes and redirections

    in part 2 remove white space on both left and right side of array. trim both sides.

    do that before starting exec. "echo        hello       world" prints out "hello world" with no white space.

    if you dont get this done, part3 WILL fail.
*/
