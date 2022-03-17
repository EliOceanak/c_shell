//wyshell.c
//Eli Oceanak
//5/7/21
//hw10
//added redirection to files
//pipes not implemented/working

#include<string.h>
#include<scannerFunctions.h>
#include<wyscanner.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){

    char buff[256];
    printf("$> ");
    

    while (fgets(buff,256,stdin)){
        //main loop
        struct command com1[100];

        struct command *comPtr, com;
        comPtr = &com1[0];
        comPtr->args[1] = NULL;
        int comcount = 0;
        int example = parse_line(buff);
        int examplecopy = 199;
        if (example == 90){
            perror("system error");
            return -1;
        }
        if (example == 96 || example == 97){
            continue;
        }
        char lexeme2[256];
        com.argcount = 1;
        while (example != 99){

            char * token;
            token = tokenMap(example);
            
            if (examplecopy > 106 && examplecopy != 200){
                //checks what the current string is, command or otherwise
                if (examplecopy == 107){
                comPtr->pipeOut = 1;
                }
                if (comcount > 0){
                    comPtr = &com1[comcount];
                }
                if (examplecopy == 107){
                    comPtr->pipeIn = 1;
                }
                strncpy(comPtr->name,lexeme,100);
                char * thisArg = malloc(4096);
                strcpy(thisArg, lexeme);
                comPtr->args[0] = thisArg;
                comPtr->argcount = 1;
                comcount = comcount + 1;
            } else if (examplecopy == 101){
                strncpy(comPtr->input_redir, lexeme, 100);
                //printf("< %s \n", token);
            } else if (examplecopy == 100){
                strncpy(comPtr->output_redir, lexeme, 100);
            } else if (examplecopy == 103){
                strncpy(comPtr->err_redir, lexeme, 100);
            } else if (example == 105){
                strncpy(comPtr->err_redir, comPtr->output_redir, 100);
            } else if (example == 200) {
                char * thisArg = malloc(4096);
                strcpy(thisArg, lexeme);
                comPtr->args[comPtr->argcount] = thisArg;
                comPtr->argcount = comPtr->argcount + 1;
            } 
            memcpy(lexeme2, lexeme, sizeof(lexeme));
            examplecopy = example;
            example = parse_line(NULL);
            if (example == 90){
               perror("system error");
               return -1;
            }
            if (example == 96 || example == 97){
                break;
            } 
            if (strcmp(lexeme2,lexeme) == 0 && examplecopy == example){
                break;
            }
        }
        if (example == 96 || example == 97){
                continue;
            } 
        comPtr->args[comPtr->argcount+1] = NULL;

        int endpipes[2];
        pipe(endpipes);
        char buffer[4096];
        for (int i = 0; i < comcount; i++){
            //redirects and piping
            int inputr = 0;
            int outputr = 0;
            int errr = 0;
            if (strcmp(com1[i].input_redir, "")){
                inputr = 1;
            }
            if (strcmp(com1[i].output_redir, "")){
                outputr = 1;
            }
            if (strcmp(com1[i].err_redir, "")){
                errr = 1;
            }
            FILE * outputFile;
            FILE * inputFile;
            FILE * errFile;
            int outfd = 0;
            int inputfd = 0;
            int errfd = 0;
            if (outputr > 0){
                if (outputFile = fopen(com1[i].output_redir, "a+")){
                    outfd = fileno(outputFile);
                } else {
                    //printf("output file could not be opened or created.");
                    break;
                }
            }
            if (inputr > 0){
                if (inputFile = fopen(com1[i].input_redir, "w+")){
                    inputfd = fileno(inputFile);
                    //printf("input file opened succesffuly\n");
                } else {
                    //printf("input file could not be opened or created.");
                    break;
                }
            }
            if (errr > 0){
                if (errFile = fopen(com1[i].err_redir, "a+")){
                    errfd = fileno(errFile);
                    //printf("error file opened succesffuly\n");
                } else {
                    //printf("error file could not be opened or created.");
                    break;
                }
            }
            
            //where most of the executing/piping happens
            pid_t child;
            child = fork();
            if (child < 0){
                printf("unable to execute this command\n");
            } else if (child == 0){
                if (com1[i].pipeIn == 1){
                    dup2(endpipes[2],(i+1)+20);
                }
                if (com1[i].pipeOut == 1){
                    //fread(buffer, sizeof(buffer), 1, endpipes[0]);
                }
                if (outfd != 0){
                    dup2(outfd,i+1);
                }
                if (errfd != 0){
                    dup2(errfd, (i+1)+10);
                }
                char buff[4096];
                if (inputfd !=0){
                    fread(buff, sizeof(buff), 1, inputFile);
                    //com1[i].args[com1[i].argcount+1] = buff;
                }
                //executes command
                int a = execvp(com1[i].name, com1[i].args);
            } else if (waitpid(child, NULL, 0) < 0) {
                perror("child never finished, exiting\n");
                break;
            }
            if (outfd != 0)
                fclose(outputFile);
        }
        //get rid of arguments
        for (int i = 1; i < comPtr->argcount; i ++){
            comPtr->args[i] = NULL;
        }
        comPtr->argcount = 0;

        printf("$> ");
        
    }

    return -1;
}