//scannerFunctions.c
//Eli Oceanak
//COSC 3750 Spring 2021
//hw8 extra scannerFunctions

#include"scannerFunctions.h"


char *tokenMap(int tokenID) {
    //takes a token and returns the correct string based on what it is
    switch (tokenID) {
        case 96:
        return "quote error";
        break;
        
        case 100 :
        return ">";
        break;

        case 102 :
        return ">>";

        case 103 :
        return "2>";

        case 104 :
        return "2>>";

        case 105 :
        return "2>&1";

        case 106 :
        return ";";

        case 108 :
        return "&";

        case 200 :
        return ":--:";
        
        case 107 :
        return "|";

        case 99 :
        return ":--: EOL";

        case 101 :
        return "<";

        default :
        return "error character: 27";

    }
}

int writeCommand(struct command * com, char * arg){
    return 1;
}