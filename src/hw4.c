#include "hw4.h"

char* prevLocation = NULL;
sigset_t mask;
pid_t pid;

void child(int sig){
    kill(pid, sig);
    exit(0);
}

void handler(int sig){
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);

    return;
    if(sig == SIGTSTP){
    printf("\ngoing to kill\n");
    //sigaddset(&mask, SIGTSTP);
    //sigprocmask(SIG_UNBLOCK, &mask, NULL);
    if(pid == 0){
        pid = getpid();
    }
    //exit(0);
    kill(pid,sig);
    kill(getpid(), SIGCHLD);
    //sigprocmask(SIG_BLOCK, &mask, NULL);
    printf("\nkilled\n");
    return;
}
else{
    printf("implement later");
}
    //while((pid = waitpid(-1, NULL, 0)) > 0){
    //    sigsuspend(&mask);
    //}
printf("did it reach here?");
}

int containsPartThree(char* input){
    int i;
    int count = 0;
    for(i = 0; *(input + i) != '\0'; i++){
        if(*(input + i) == '>' || *(input + i) == '<' || *(input + i) == '|'){
            count = count + 1;
        }
    }
    return count;
}

int countChar(char* string, char c){
    int count = 0;
    int i = 0;
    while(*(string + i) != '\0'){
        if(*(string + i) == c){
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

char* splitString(char* string, char c){
    int buffLen = countChar(string, c) + 1;
    char* ptr[buffLen + 1];
    //char** ret = malloc(buffLen);
    char* buffer = malloc(strlen(string) + 2);
    int len = strlen(string);
    int j = 1;
    buffer = strcpy(buffer, string);
    ptr[0] = buffer;
    int i;
    for(i = 0; i < len; i++){
        if(*(string + i) == c){
            *(buffer + i) = '\0';
            ptr[j++] = buffer + i + 1;
        }
    }
    *(buffer + i) = '\0';
    ptr[j++] = buffer + i + 1;
    ptr[j] = NULL;
    //buffer = memcpy(buffer, ptr, strlen(string) + 1);
    *ptr = buffer;
    return *ptr;
}

char* newArgs(char* ptr[], char* buffer, int num){
    int len = strlen(buffer);
    for(int i = 1; ptr[i] != NULL; i++){
        len += strlen(ptr[i]) + 1;
    }
    char* argv = malloc(len + 2);
    argv = strcpy(argv, buffer);
    if(num > 1){
        memcpy((argv + strlen(buffer) + 1), ptr[1] , len - strlen(buffer));
    }else{
        memcpy((argv + strlen(buffer) + 1), ptr[0] , len - strlen(buffer));
    }
    free(ptr[0]);
    char* nullptr = argv + 1 + len;
    nullptr = nullptr;
    nullptr = NULL;

    return argv;
}

int containsSlash(char* string){
    int ret = 0;
    for(int i = 0; *(string + i) != '\0' && *(string + i) != ' '; i++){
        if(*(string + i) == '/'){
            ret = ret + 1;
        }
    }

    return ret;
}

int withWhite(char* input, int len){
    input = input + len;
    return *input == ' ' || *input == '\t' || *input == '\0' || *input == '\n';
}

char* getdir(int len){
    char* buf = NULL;
    buf = getcwd(NULL, 0);

    if(strncmp(buf, getenv("HOME"),len) == 0){
        *(buf + len - 1) = '~';
        char* toCopy = (buf + len - 1);
        buf = strcpy(buf, toCopy);
    }
    //free(buf);
    return buf;
}

char* removeWhiteSpace(char* input){
    int i = 0;
    while(*(input + i) == ' '){
        i = i + 1;
    }
    return input + i;
}

void ischdir(char* input){
    char* prev = getcwd(NULL, 0);
    if(strcmp(input, "cd") == 0){
        chdir(getenv("HOME"));
        free(prevLocation);
        prevLocation = prev;
    }else if(strncmp(input, "cd -", 4) == 0){
        chdir(prevLocation);
        free(prevLocation);
        prevLocation = prev;
    }else if(strcmp(input, "cd") > 0){
        input = removeWhiteSpace(input + 2);
        chdir(input);
        free(prevLocation);
        prevLocation = prev;
    }else if(strncmp(input, "cd ..", 5) == 0){
        chdir("..");
        free(prevLocation);
        prevLocation = prev;
    }else if(strncmp(input, "cd .", 4) == 0){
        chdir(".");
        free(prevLocation);
        prevLocation = prev;
    }
}

void help(){
    char *help = "help: Print a list of all builtins and their basic usage in a single column.\n"\
"exit: Exits the shell\n"\
"cd: Changes the current working directory of the shell.\n"\
"\tcd - should change the working directory to the last directory the user\n\twas in.\n"\
"\tcd with no arguments should go to the user's home directory.\n"\
"\tcd . should stay in the current directory\n"\
"\tcd .. should go to the previous directory of the current working\n\tdirectory.\n"\
"pwd: Prints the absolute path of the current working directory.\n"\
;

printf("%s", help);

}

char* tightenPipesAndCarrots(char* newInput){
    char* input = malloc(strlen(newInput) + 1);
    int i = 0;
    int j = 0;
    while(*(newInput + i) != '\0'){
        if(*(newInput + i) == ' ' && (*(newInput + i + 1) == '>' || *(newInput + i + 1) == '<' || *(newInput + i + 1) == '|' )){
            i++;
            *(input + j++) = *(newInput + i);
            if(*(newInput + i + 1) == ' '){
                i++;
            }

        }else{
            *(input + j++) = *(newInput + i);
        }
        i++;
    }
    *(input + j++) = '\0';
    free(newInput);
    return input;
}

char* trimString(char* input){
    //char buffer[strlen(input)];
    char* newInput = malloc(strlen(input) + 1);
    int i = 0;
    //strcpy(buffer, input);
    if(*input == ' '){
        while(*(input + i) == ' '){
            i++;
        }
    }
    int j = 0;
    while(*(input + i) != '\0'){
        if(*(input + i) == ' '){
            *(newInput + j++) = *(input + i++);
            while(*(input + i) == ' '){
                i++;
            }
        }else{
            *(newInput + j++) = *(input + i);
            i++;
        }
    }
    *(newInput + j) = '\0';
    free(input);
    newInput = tightenPipesAndCarrots(newInput);
    return newInput;
}

int redirectOrPipe(char* input, char* envp[]){
    int pwdOrHelp = 0;
    int status = 0;
    if(strncmp(input, "help", 4) == 0){
        pwdOrHelp = 1;
    }else if(strncmp(input, "pwd", 3) == 0){
        pwdOrHelp = 2;
    }

    if(countChar(input, '<') > 1 || countChar(input, '>') > 1){
        printf("sfish syntax error: %s : to many pipe redirections, only one\n", input);
            return -1;
    }

    char* toFree;
    if(countChar(input, '<') && countChar(input, '>')){
        if(countChar(input,('|'))){
            printf("sfish syntax error: %s :no pipe with redirect\n", input);
            return -1;
        }
        int inOrOut = -1; //in first = 0 out first = 1
        char one;
        char two;
        for(int i = 0; *(input + i) != '\0'; i++){
            if(*(input + i) == '<'){
                inOrOut = 0;
                one = '>';
                two = '<';
                break;
            }else if(*(input + i) == '>'){
                inOrOut = 1;
                one = '<';
                two = '>';
                break;
            }
        }
        char* splitOne;
        char* splitTwo;
        char* toFree2;
        splitOne = splitString(input, one);
        char* buffer[3];
        *buffer = malloc(strlen(input) + 4);
       toFree = *buffer;
       int j = 0;
       buffer[j++] = splitOne;
       for(int i = 0; *(input + i) != '\0'; i++){
            if(*(input + i) == one){
                buffer[j++] = (splitOne + i + 1);
            }
       }
       buffer[j] = NULL;
       splitTwo = splitString(buffer[0], two);
       char* buffer2[4];
       *buffer2 = malloc(strlen(input) + 4);
       toFree2 = *buffer2;
       j = 0;
       buffer2[j++] = splitTwo;
       for(int i = 0; *(input + i) != '\0'; i++){
            if(*(input + i) == two){
                buffer2[j++] = (splitTwo + i + 1);
            }
       }
       buffer2[j++] = buffer[1];
       buffer2[j++] = NULL;

       status = -2;
       pid_t pid;
       if((pid = fork()) == 0){
            int infd;
            int outfd;
            if(inOrOut == 0){
                infd = open(buffer2[1], O_RDONLY);
                outfd = open(buffer2[2], O_WRONLY);
            }else if(inOrOut == 1){
                infd = open(buffer2[2], O_RDONLY);
                outfd = open(buffer2[1], O_WRONLY);
            }
            int stdin = dup(STDIN_FILENO);
            int stdout = dup(STDOUT_FILENO);
            infd = dup2(infd, STDIN_FILENO);
            outfd = dup2(outfd, STDOUT_FILENO);
            myExec(buffer2[0], envp);
            dup2(stdin, infd);
            dup2(stdout, outfd);
            close(infd);
            close(outfd);
            exit(0);
       }

       waitpid(pid, &status, 0);

        free(splitOne);
        free(splitTwo);
        free(toFree);
        free(toFree2);
    }else if(countChar(input,('<'))){
         if(countChar(input,('|'))){
            printf("sfish syntax error: %s: no pipe with redirection\n", input);
            return -1;
        }
       char* inRedirect = splitString(input, '<');
       char* buffer[3];
       *buffer = malloc(strlen(input) + 1);
       toFree = *buffer;
       int j = 0;
       buffer[j++] = inRedirect;
       for(int i = 0; *(input + i) != '\0'; i++){
            if(*(input + i) == '<'){
                buffer[j++] = (inRedirect + i + 1);
            }
       }
       buffer[j] = NULL;
       int status = -2;
       pid_t pid;
       if((pid = fork()) == 0){
            int infd = open(buffer[1], O_RDONLY);
            int std = dup(STDIN_FILENO);
            infd = dup2(infd, STDIN_FILENO);
            myExec(buffer[0], envp);
            dup2(std, infd);
            close(infd);
            exit(0);
       }

        waitpid(pid, &status, 0);
        free(toFree);
       free(inRedirect);
    }else if(countChar(input,('>'))){
        if(countChar(input,('|'))){
            printf("sfish syntax error: %s: no pipe with redirection\n", input);
            return -1;
        }
        char* outRedirect = splitString(input, '>');
        char* buffer[3];
       *buffer = malloc(strlen(input) + 1);
       toFree = *buffer;
       int j = 0;
       buffer[j++] = outRedirect;
       for(int i = 0; *(input + i) != '\0'; i++){
            if(*(input + i) == '>'){
                buffer[j++] = (outRedirect + i + 1);
            }
       }
       buffer[j] = NULL;
       pid_t pid;
       if((pid = fork()) == 0){
            int outfd = open(buffer[1], O_WRONLY, O_CREAT);
            int std = dup(STDOUT_FILENO);
            outfd = dup2(outfd, STDOUT_FILENO);
            if(pwdOrHelp == 0){
                myExec(buffer[0], envp);
            }else if(pwdOrHelp == 1){
                help();
            }else if(pwdOrHelp == 2){
                char* temp = getcwd(NULL, 0);
                printf("%s\n", temp);
                free(temp);
            }
            dup2(std, outfd);
            close(outfd);
            exit(0);
       }
        waitpid(pid, &status, 0);
        free(toFree);
        free(outRedirect);
    }else if(countChar(input, ('|'))){
        if(countChar(input,('>')) || countChar(input,('<'))){
            printf("sfish syntax error: %s: no redireciton with pipe\n", input);
            return -1;
        }

        char* pipes = splitString(input, '|');
        int programNum = countChar(input, '|') + 1;

        //pid_t pid;
        char* newInput[programNum + 1];

        int j = 0;
        newInput[j++] = pipes;
        int i = 0;
        while(j < programNum){
            if(*(pipes + i++) == '\0'){
                newInput[j++] = (pipes + i);
            }
        }
        newInput[j++] = NULL;
        //sigset_t mask;
        //signal(SIGCHLD, handler);
       //sigset_t prev;
       int evenPipe[2];
       int oddPipe[2];

       pipe(evenPipe);

        for(i = 0; i < programNum; i++){
            sigemptyset(&mask);
            //sigaddset(&mask, SIGCHLD);
            sigaddset(&mask, SIGTSTP);
            sigaddset(&mask, SIGINT);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            if((pid = fork()) == 0){
                if(i != 0 && i != programNum - 1){
                    if(i%2 == 0){
                        dup2(oddPipe[0], STDIN_FILENO);
                        dup2(evenPipe[1], STDOUT_FILENO);
                        close(oddPipe[1]);
                        close(evenPipe[0]);
                    }else{
                        dup2(evenPipe[0], STDIN_FILENO);
                        dup2(oddPipe[1], STDOUT_FILENO);
                        close(oddPipe[0]);
                        close(evenPipe[1]);
                    }
                }else if(i == 0){
                    dup2(evenPipe[1], STDOUT_FILENO);
                    close(evenPipe[0]);
                }else if(i == programNum - 1){
                    if(i%2 == 0){
                        dup2(oddPipe[0], STDIN_FILENO);
                        close(oddPipe[1]);
                        close(evenPipe[0]);
                        close(evenPipe[1]);
                    }else{
                        dup2(evenPipe[0], STDIN_FILENO);
                        close(evenPipe[1]);
                        close(oddPipe[1]);
                        close(oddPipe[0]);
                    }
                }
                sigprocmask(SIG_UNBLOCK, &mask, NULL);
                myExec(newInput[i], envp);
                close(evenPipe[1]);
                close(evenPipe[0]);
                close(oddPipe[0]);
                close(oddPipe[1]);
                exit(0);
            }


            if(i%2 == 0){
                if(i != 0){
                    close(oddPipe[0]);
                }
                close(evenPipe[1]);
                if(i != programNum - 1){
                    pipe(oddPipe);
                }else{
                    close(oddPipe[1]);
                    close(evenPipe[0]);
                }
            }else{
                close(oddPipe[1]);
                close(evenPipe[0]);
                if(i != programNum - 1){
                    pipe(evenPipe);
                }else{
                    close(oddPipe[0]);
                    close(evenPipe[1]);
                }
            }
            //sigprocmask(SIG_UNBLOCK, &mask, NULL);
            //sigdelset(&mask, SIGCHLD);
            //sigprocmask(SIG_BLOCK, &mask, NULL);
            //sigemptyset(&mask);
            //sigaddset(&mask, SIGCHLD);
            //sigaddset(&mask, SIGTSTP);
            //sigaddset(&mask, SIGINT);

            signal(SIGINT, handler);
            signal(SIGTSTP, handler);
            waitpid(pid, NULL, 0);
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            //waitpid(-1, NULL, 0);
            //sigdelset(&mask, SIGINT);
            //sigdelset(&mask, SIGTSTP);
            //sigsuspend(&mask);

        }
        //sigdelset(&mask, SIGCHLD);
        //waitpid(pid, NULL, 0);
        free(pipes);
    }else{
        printf("\n%s\n", "Should not have reached this point");


        return -1;
    }
    return status;
}

int myExec(char* input,  char* envp[]){
                pid_t pid;
                int status = -1;
                struct stat *currentlyUseless = malloc(sizeof(struct stat));
                char* buffer = malloc(1);

                char* preArgv[countChar(input, ' ') + 1];
                char* argv = splitString(input, ' '); //used as argv in execve
                int j = 0;
                char* temp;
                preArgv[j++] = argv;
                for(int i = 0; *(input + i) != '\0'; i++){
                    if(*(input + i) == ' '){
                    temp = argv + i + 1;
                    preArgv[j++] = temp;
                }
                }
                preArgv[j] = NULL;

            if(containsSlash(input) > 0){ // if it contains a slash it will do the code below
                char* pwd = getcwd(NULL, 0);
                free(buffer);
                buffer = malloc(strlen(input) + strlen(pwd) + 2);
                buffer = strcpy(buffer, pwd);
                buffer = strcat(buffer, "/");
                buffer = strcat(buffer, preArgv[0]);
                status = stat(preArgv[0], currentlyUseless);
                free(pwd);

            }else{
                char* envString = getenv("PATH");
                char *envBuf = malloc(strlen(envString) + 1);
                envBuf = strncpy(envBuf, envString, strlen(envString) + 1);

                char* in = splitString(envBuf, ':');
                char* inStart = in;
                int j = 0;
                while(*in != '\0'){

                    free(buffer);
                    buffer = malloc(strlen(getenv("PATH"))+ strlen(preArgv[0]));
                    buffer = strcpy(buffer, in);//puts in an env path, in my case /bin
                    buffer = strcat(buffer, "/");   //now /bin/
                    buffer = strcat(buffer, preArgv[0]); //now /bin/ls

                    if((status = stat(buffer, currentlyUseless)) == 0){ //returns 0 for /bin/ls
                        break;
                    }
                    for(j = 0; *(in + j) != '\0'; j++){
                        //do nothing...
                    }

                    in = (in + j + 1);

                }

                free(envBuf);
                free(inStart);

            }
            if(status == 0 && (pid = fork() == 0)){
                int myArgvLen = countChar(input, ' ') + 1;
                argv = newArgs(preArgv, buffer, myArgvLen);
                preArgv[0] = argv;
                char* myArgv[myArgvLen + 1];
                int i = 0;
                j = 0;
                myArgv[j++] = argv;
                char* ptr;

                while(j < myArgvLen){
                    if(*(argv + i++) == '\0'){
                        ptr = argv + i;
                        myArgv[j++] = ptr;
                    }
                }
                myArgv[j] = NULL;
                //char* argv = newArgs(preArgv, buffer);
                //preArgv = &argv;
                //signal(SIGTSTP, child);
                //signal(SIGINT, child);
                execve(myArgv[0], myArgv, envp);// buffer contains /bin/ls
               // //char* pre = "#!";
                //buffer = strcat(pre, buffer);
                //execv(buffer, preArgv);
                //execl(buffer, *preArgv, 0, 0);
               // debug("ERROR");
                //printf("\n%d\n", errno);
                exit(-1);
            }else if(status == -1){
                printf(EXEC_NOT_FOUND, input);
            }

            signal(SIGTSTP, child);
            signal(SIGINT, child);
            sigdelset(&mask, SIGCHLD);
            //sigprocmask(SIG_BLOCK, &mask, NULL);
            wait(&status);
            //sigprocmask(SIG_UNBLOCK, &mask, NULL);
            signal(SIGTSTP, SIG_DFL);
            free(preArgv[0]);

            free(buffer);
            free(currentlyUseless);

            return status;
}