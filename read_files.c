//
// Created by linux on 8/14/20.
//

#include "read_files.h"
#include "first_transition.h"
#include "second_transition.h"
#include "code_func.h"
#include <string.h>
#include <stdlib.h>

bool isValidFile;
extern size_t ic;
extern size_t dc;
char *fileName2;

void printError(Error err, size_t numLine)
{
    char * errorMsg[7] = {"invalid syntax", "Illegal addressing method for the first operand",
                          "Illegal addressing method for the second operand",
                          "no such symbol", "file does not exist", "redeclaration of symbol",
                          "too many operands"};
    printf("error in file %s, line %ld: %s\n", fileName2, numLine, errorMsg[err]);
}



void error(Error err, size_t numLine)
{
    isValidFile = false;
    printError(err, numLine);
}

void printLineNum(int num, FILE *fp)
{
    fprintf(fp,"0000");
    fprintf(fp,"%d\t", num);
}


void printBinaryRepresentation(int number, size_t j, FILE *fp)
{
    unsigned int musk = 1 << j - 1;
    int i;

    for (i = 0; i < j; ++i, musk = musk >> 1)
    {
        musk & number? printf("%d", 1):printf("%d", 0);
    }
}



void printHexNum(int word, FILE *fp)
{
    int musk = 15 << 20;

    for (size_t i = 0; i < 6; i++, musk = musk >> 4)
    {
        fprintf(fp,"%x", (word&musk)>>(20 - 4*i));
    }
    fprintf(fp,"\n");
}

void printToFile(int * output)
{
    FILE *fp = fopen("output.txt", "a");
    fprintf(fp, "     %ld\t%ld\n", ic - 100, dc);
    for(int i = 0; i < (ic + dc - 100); i++)
    {
        printLineNum(i + 100, fp);
        printHexNum(output[i], fp);
        printBinaryRepresentation(output[i], 24, fp);
        printf("\n");
    }
}



void read_files(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        int len = strlen(argv[i]);
        const char *last_three = &argv[i][len - 3];
        if (!strcmp(last_three, ".as"))
        {
            fileName2 = argv[i];
            char *fileName = str_slice(argv[i], 0, len - 2);
            fileName = strcat(fileName, "txt");
            isValidFile = true;
            first_iteration(fileName);
            int * output = second_iteration(fileName);
            if(isValidFile)
                printToFile(output);
            free(output);
        }
    }
}