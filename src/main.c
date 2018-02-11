#include "hw4.h"


int main(int argc, char *argv[], char* envp[]) {
    char* input;
    bool exited = false;
    int homeLen = strlen(getenv("HOME"));
    int promptOn =  1;
    prevLocation = getcwd(NULL, 0);
    char* rightSide = " :: ssimione >> ";
    char* getColor;
    char* color = "\x1B[0m";
    if(!isatty(STDIN_FILENO)) {
        promptOn = 0;
        // If your shell is reading from a piped file
        // Don't have readline write anything to that file.
        // Such as the prompt or "user input"
        if((rl_outstream = fopen("/dev/null", "w")) == NULL){
            perror("Failed trying to open DEVNULL");
            exit(EXIT_FAILURE);
        }
    }

    do {
        char* leftSide = getdir(homeLen);
        if(promptOn){
            char* prompt =  malloc(strlen(leftSide) + strlen(rightSide));
            prompt = strcpy(prompt, leftSide);
            prompt = strcat(prompt, rightSide);
            printf("%s", color);
            printf("%s", prompt);
            input = readline("\x1B[0m");
            printf("\x1B[0m");
            free(prompt);
            free(leftSide);
        }else{
            input = readline("");
        }

        write(1, "\e[s", strlen("\e[s"));
        write(1, "\e[20;10H", strlen("\e[20;10H"));
        write(1, "SomeText", strlen("SomeText"));
        write(1, "\e[u", strlen("\e[u"));

        //input = removeWhiteSpace(input);
        input = trimString(input);



        // If EOF is read (aka ^D) readline returns NULL
        if(input == NULL) {
            continue;
        }
        if(*input == 'c' && *(input + 1) == 'd'){
            ischdir(input);
        }else if(strncmp(input, "help", 4) == 0 && withWhite(input, 4)){
            help();
        }else if(strncmp(input, "pwd", 3) == 0 && withWhite(input, 3)){
            char* temp = getcwd(NULL, 0);
            printf("%s\n", temp);
            free(temp);
        }else if(strncmp(input, "exit", 4) == 0 && withWhite(input, 4)){
            // You should change exit to a "builtin" for your hw.
            exited = 1;
        }else if(strncmp(input, "color", 5) == 0 && withWhite(input,5)){
            getColor = splitString(input, ' ');
            int i = 0;
            while(*(getColor + i++)  != '\0'){
                //do nothing
            }
            color = (getColor + i);
            if(strcmp(color, "RED") == 0){
                color = "\033[1;31m";
            }else if(strcmp(color, "GRN") == 0){
                color = "\033[1;32m";
            }else if(strcmp(color, "YEL") == 0){
                color = "\033[1;33m";
            }else if(strcmp(color, "BLU") == 0){
                color = "\033[1;34m";
            }else if(strcmp(color, "MAG") == 0){
                color = "\033[1;35m";
            }else if(strcmp(color, "CYN") == 0){
                color = "\033[1;36m";
            }else if(strcmp(color, "WHT") == 0){
                color = "\033[1;37m";
            }else if(strcmp(color, "BWN") == 0){
                color = "\033[0;33m";
            }
        }else if(containsPartThree(input) > 0){
            redirectOrPipe(input, envp);
        }else{

            sigemptyset(&mask);
            sigaddset(&mask, SIGTSTP);
            //sigaddset(&mask, SIGCHLD);
            sigaddset(&mask, SIGINT);
            //sigprocmask(SIG_BLOCK, &mask, NULL);
            if((pid = fork()) == 0){
                //signal(SIGTSTP, handler);
                //signal(SIGINT, handler);
                myExec(input, envp);
                exit(0);
            }
            signal(SIGTSTP, handler);
            signal(SIGINT, handler);

            //sigprocmask(SIG_UNBLOCK, &mask, NULL);
            //sigprocmask(SIG_BLOCK, &mask, NULL);
            waitpid(pid, NULL, 0);
            //sigprocmask(SIG_UNBLOCK, &mask, NULL);
            //sigprocmask(SIG_UNBLOCK, &mask, NULL);
            //signal(SIGTSTP, SIG_DFL);
            //waitpid(-1, NULL, 0);
            //myExec(input, envp);;
        // Currently nothing is implemented
        }

        // Readline mallocs the space for input. You must free it.
        rl_free(input);

    } while(!exited);

    debug("%s", "user entered 'exit'");

    free(getColor);
    free(prevLocation);

    return EXIT_SUCCESS;
}

