//
// Created by linux on 8/10/20.
//

#include "first_transition.h"
#include "code_func.h"
#include <stdlib.h>
#include <string.h>
#include "guidance_sen.h"
#include "directive_sen.h"
#include "read_files.h"

size_t ic = 100;
size_t dc = 0;
size_t cnt = 0;
size_t numEntries = 0;
size_t numLines = 0;
size_t numGuidance = 0;



void checkLine(const char * line, size_t lineNum);

Symbol *symbolTable;
DirectiveLine *memory;
GuidanceLine *data;
EntrySymbol *entrySymbols;



int isInTable(const char * symbol)
{
    size_t i;
    for(i = 0; i < cnt; i++)
    {
        if (!strcmp(symbolTable[i].symbolName, symbol))
        {
            return i;
        }

    }
    return -1;
}



bool isGuidance(const char* line, size_t i)
{
    while(line[i] && line[i] != '.'){i++;}
    return line[i] && line[i] == '.';
}


void insertSymbolName(const char * name)
{
    cnt++;
    symbolTable = (Symbol *) realloc(symbolTable, cnt * sizeof(Symbol));
    strcpy(symbolTable[cnt - 1].symbolName, name);

}

void signDirSymbol()
{
    symbolTable[cnt - 1].isInstruction = true;
    symbolTable[cnt - 1].address = ic;
}


void signGuidSymbol()
{
    symbolTable[cnt - 1].isInstruction = false;
    symbolTable[cnt - 1].address = dc;
}


void insertSymbol(const char * line, size_t i, size_t j, size_t lineNum)
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
        symbolTable[cnt - 1].type = 2;
        checkLine(str_slice(line, j + 1, strlen(line)), lineNum);
    }
    else
        error(E_SYNTAX, lineNum);

}



void label(const char * line, size_t i, size_t lineNum) {
    size_t j = i;

    while (line[j] && (line[j] != ':'))
    {
        j++;
    }
    if(isInTable(str_slice(line, i, j)) == -1)
    {
        insertSymbol(line, i, j, lineNum);
    }
    else
        error(E_SYNTAX, lineNum);
}




void checkLine(const char * line, size_t lineNum)
{
   
    size_t i = 0;
    while(line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;

    if (line[i] == ';')
        return;

    if (line[i] == '.')
        guidanceSen(line, i, lineNum);

    else if ((i + 3 < strlen(line) && line[i + 3] == ' ' && getOpcode(str_slice(line, i, i + 3)))
             || i + 4 < strlen(line) && !strcmp("stop", str_slice(line, i, i + 4)))
        directiveSen(line, i, lineNum);

    else
        label(line, i, lineNum);

}



void initDataTables()
{
    memory = (DirectiveLine *)malloc(sizeof(DirectiveLine));
    data = (GuidanceLine*)malloc(sizeof(GuidanceLine));
    symbolTable = (Symbol*)malloc(sizeof(Symbol));
    entrySymbols = (EntrySymbol*)malloc(sizeof(EntrySymbol));
}


void signEntrySymbols()
{
    for (int i = 0; i < cnt; ++i)
    {
        if (!symbolTable[i].isInstruction && symbolTable[i].type != 1)
        {
            symbolTable[i].address += ic;
        }
    }
}


void first_iteration(const char * fileName) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t lineNum = 0;
    fp = fopen(fileName, "r");

    if (fp == NULL){
        exit(EXIT_FAILURE);
    }

    initDataTables();
    while ((getline(&line, &len, fp)) != -1)
    {
        lineNum++;
        checkLine(line, lineNum);
    }


    fclose(fp);
    if (line)
        free(line);
    signEntrySymbols();
}

