//scannerFunctions.h
//Eli Oceanak
//COSC 3750 Spring 2021
//extra functions for wyscanner


char *tokenMap (int tokenID);

struct command {
    //char * name;
    char name[100];
    //you only get a maximum of 100 arguments. Setting this # higher was screwing up
    //for some reason
    char * args[100];
    //having it be a char ** was giving me an insane amount of trouble
    //char** args;
    char input_redir[100];
    char output_redir[100];
    char err_redir[100];
    int argcount;
    int pipeIn;
    int pipeOut;
};

int writeCommand(struct command * com, char * arg);

//struct Command;