
#include "first_transition.h"
#include "directives_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "guidance_sen.h"
#include "directive_sen.h"
#include "read_write_files.h"

#define MAX_LINE_LEN 80

/*----------------Defining global variables of the system----------------*/

/*----Setting up counters----*/
size_t ic = START;
size_t dc = 0, symbolNum = 0, numEntries = 0, numLines = 0, numGuidance = 0;

/*----Define tables----*/
Symbol *symbolTable;
DirectiveLine *memory;
GuidanceLine *data;
ExternSymbol *entrySymbols;


static void checkLine(const char * line, size_t lineNum);


/*----------------Auxiliary functions-------------*/


/*The function takes a string, and checks if there is a symbol
  whose name is the same as the string -
  if so, it returns its index in the table. Otherwise - it returns -1.*/
int isInTable(const char * symbol)
{
    size_t i;
    for(i = 0; i < symbolNum; i++)
    {
        if (!strcmp(symbolTable[i].symbolName, symbol))
        {
            return i;
        }

    }
    return -1;
}

/*The function takes a string of a line,
 and returns a Boolean value that indicates
 whether the line is a guidance sentence or not.*/
static bool isGuidance(const char* line, size_t i)
{
    while(line[i] && line[i] != '.'){i++;}
    return line[i] && line[i] == '.';
}


/* The function gets a string of a symbol name, and inserts it into the field 'symbolName'.*/
static void insertSymbolName(const char * name)
{
    symbolNum++;
    symbolTable = (Symbol *) realloc(symbolTable, symbolNum * sizeof(Symbol));
    strcpy(symbolTable[symbolNum - 1].symbolName, name);

}

/*Indicate that the symbol define a directive sentence and update the symbol address. */
static void signDirSymbol()
{
    symbolTable[symbolNum - 1].isDirective = true;
    symbolTable[symbolNum - 1].address = ic;
}

/*Indicate that the symbol define a guidance sentence and update the symbol address.. */
static void signGuidSymbol()
{
    symbolTable[symbolNum - 1].isDirective = false;
    symbolTable[symbolNum - 1].address = dc;
}


/*The function inserts a new symbol into the symbol table and initializes the required fields.*/
static void insertSymbol(const char * line, size_t i, size_t j, size_t lineNum)
{
    if (line[j] && line[j] == ':')
    {
            insertSymbolName(str_slice(line, i, j));
            if (!isGuidance(line, j))
            {
                signDirSymbol();
            }
            else
            {
                signGuidSymbol();
            }
            symbolTable[symbolNum - 1].isExtern = 2;
            /*Continue to check and analyze the continuation of the line after the label is set*/
            checkLine(str_slice(line, j + 1, strlen(line)), lineNum);
    }
    else
        error(E_SYNTAX, lineNum);
}


/*The function takes a line that defines a label, or is invalid.
  The function checks if there is a label setting -
  if so, it inserts the label to the table, otherwise - it takes out an error.*/
static void label(const char * line, size_t i, size_t lineNum) {
    size_t j = i;

    while (line[j] && (line[j++] != ':')){}
    j--;
    /*Checks if the label has already been set or not.*/
    if(isInTable(str_slice(line, i, j)) == -1)
        insertSymbol(line, i, j, lineNum);

    else
        error(E_REDECLARATION, lineNum);
}


/*The function checks the line type (directive, guidance, or blank line / note),
  and works accordingly*/
static void checkLine(const char * line, size_t lineNum)
{

    size_t i = 0;
    while(line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;

    if(line[i] == '\n')
        return;
    /*note line*/

    if (line[i] == ';') {return; }
    /*guidance line*/
    if (line[i] == '.')
        guidanceSen(line, i, lineNum);
    /*directive line*/
    else if ((i + 3 < strlen(line) && line[i + 3] == ' ' && getDirIdx(str_slice(line, i, i + 3)) != -1)
             || (i + 4 < strlen(line) && !strcmp("stop", str_slice(line, i, i + 4))))
        {
        directiveSen(line, i, lineNum);
        }

    /*definition of label*/
    else{
        label(line, i, lineNum);
    }
}

/* This function initializes the system tables */
static void initDataTables()
{
    memory = (DirectiveLine *)malloc(sizeof(DirectiveLine));
    data = (GuidanceLine*)malloc(sizeof(GuidanceLine));
    symbolTable = (Symbol*)malloc(sizeof(Symbol));
    entrySymbols = (ExternSymbol*)malloc(sizeof(ExternSymbol));
    ic = START;
    dc = symbolNum = numEntries = numLines = numGuidance = 0;
}

/*This function adds the ic to the address of non extern symbols*/
static void addIc()
{
    int i;
    for(i = 0; i < symbolNum; ++i)
    {
        if (!symbolTable[i].isDirective && symbolTable[i].isExtern != 1)
        {
            symbolTable[i].address += ic;
        }
    }
}


/* -------------- the main function in this file -------------- */

/* The function makes the first transition over the file:
 it reads line by line in the file, checks the type of sentence,
 saves the directives and guidance sentences separately and also builds a table of symbols. */
void firstIteration(FILE *fp)
{
     char line[MAX_LINE_LEN];
     size_t lineNum = 0;


     initDataTables();
     while (fgets(line, MAX_LINE_LEN, fp))
     {
         checkLine(line, ++lineNum);
     }
     fclose(fp);
     addIc();
}