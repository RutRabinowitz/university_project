//
// Created by linux on 8/14/20.
//

#include "read_files.h"
#include "first_transition.h"

bool isValidFile;


void printError(Error err, size_t numLine)
{
    char * errorMsg[4] = {"invalid syntax", "first operand is not valid", "second operand is not valid", "symbol"};
    printf("error in line %d: %s\n", numLine, errorMsg[err]);
}



void error(Error err, size_t numLine)
{
    isValidFile = false;
    printError(err, numLine);
}



void read_files(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        int len = strlen(argv[i]);
        const char *last_three = &argv[i][len - 3];
        if (!strcmp(last_three, ".as"))
        {
            char *fileName = str_slice(argv[i], 0, len - 2);
            fileName = strcat(fileName, "txt");
            isValidFile = true;
            first_iteration(fileName);
            second_iteration(fileName);
            if(isValidFile)
                printf("yes\n");
        }
    }
    return 0;
}