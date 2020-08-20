
#include "read_files.h"
#include "first_transition.h"
#include "second_transition.h"
#include "code_func.h"
#include <string.h>
#include <stdlib.h>


bool isValidFile;
extern size_t ic;
extern size_t dc;
char *fileName;

extern EntrySymbol *entrySymbols;
extern size_t numEntries;
extern Symbol *symbolTable;

extern EntrySymbol* externs;
extern size_t numExterns;

void printError(Error err, size_t numLine)
{
    char * errorMsg[7] = {"Invalid syntax", "Illegal addressing method for the first operand",
                          "Illegal addressing method for the second operand",
                          "No such symbol", "file does not exist", "Redeclaration of symbol",
                          "Too many operands"};
    if (err == 4)
        printf("%s: %s\n", fileName, errorMsg[err]);
    else
        printf("file %s: %ld: %s\n", fileName, numLine, errorMsg[err]);
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
    unsigned int musk = 1 << (j - 1);
    int i;

    for (i = 0; i < j; ++i, musk = musk >> 1)
    {
        musk & number? printf("%d", 1):printf("%d", 0);
    }
}



void printHexNum(int word, FILE *fp)
{
    int musk = 15 << 20;
    size_t i;
    for(i = 0; i < 6; i++, musk = musk >> 4)
    {
        fprintf(fp,"%x", (word&musk)>>(20 - 4*i));
    }
    fprintf(fp,"\n");
}


void printToFile(int * output)
{
    FILE *fp;
    size_t i;
    char *fileName2 = (char*)malloc(sizeof(char)*(strlen(fileName) + 1));
    strcpy(fileName2, fileName);
    fp = fopen(strcat(fileName2, ".ob"), "w");
    fprintf(fp, "%7ld\t%ld\n", ic - START, dc);
    for(i = 0; i < (ic + dc - START); i++)
    {
        printLineNum(i + START, fp);
        printHexNum(output[i], fp);
    }
    free(fileName2);
}


void printEntryFile()
{
    FILE *fp;
    char *fileName2 = (char*)malloc(sizeof(char)*(strlen(fileName) + 1));
    strcpy(fileName2, fileName);
    fp = fopen(strcat(fileName2, ".ent"), "w");
    size_t i;
    for(i = 0; i < numEntries; i++)
    {
        fprintf(fp,"%s\t", entrySymbols[i].name);
        fprintf(fp,"%07ld", symbolTable[isInTable(entrySymbols[i].name)].address);
        fprintf(fp,"\n");
    }
    free(fileName2);
}


void printExtFile()
{
    size_t i;
    FILE *fp;
    char *fileName2 = (char*)malloc(sizeof(char)*(strlen(fileName) + 1));
    strcpy(fileName2, fileName);
    fp = fopen(strcat(fileName2, ".ext"), "w");
    for(i = 0; i < numExterns; i++)
    {
        fprintf(fp, "%s\t", externs[i].name);
        fprintf(fp, "%07ld", externs[i].numLine + START);
        fprintf(fp, "\n");
    }
    free(fileName2);
}


void freeTable()
{
    free(externs);
    free(entrySymbols);
    free(symbolTable);
}


void printFilesIfValid(int *output)
{
    if(isValidFile)
    {
        printToFile(output);
        printEntryFile();
        printExtFile();
    }
}


void readFiles(int argc, char *argv[])
{
    size_t i;
    int * output;

    for (i = 1; i < argc; i++)
    {
        char* file = (char*)malloc(sizeof(char)*strlen(argv[i] + 1));
        fileName = (char*)malloc(sizeof(char)*strlen(argv[i] + 1));
        strcpy(file, argv[i]);
        strcpy(fileName, argv[i]);
        strcat(file, ".as");
        isValidFile = true;
        firstIteration(file);
        output = secondIteration();
        printFilesIfValid(output);
        freeTable();
        free(output);
        free(file);
        free(fileName);
    }
}