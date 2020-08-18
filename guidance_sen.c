//
// Created by linux on 8/11/20.
//

//#include "guidance_sen.h"
#include <stdlib.h>
#include <string.h>
#include "code_func.h"
#include "first_transition.h"
#include "read_files.h"



extern Symbol *symbolTable;
extern DirectiveLine *memory;
extern GuidanceLine *data;
extern EntrySymbol *entrySymbols;


extern size_t ic;
extern size_t dc;
extern size_t cnt;
extern size_t numEntries;
extern size_t numLines;
extern size_t numGuidance;


bool isNumber(const char * str)
{
    int i, len = strlen(str);
    bool flag = true;
    if (str && str[0] == '0')
        flag = false;
    for(i = 0; i < len && flag; i++)
    {
        if(!(str[i] >= '0' && str[i] <= '9' || (i == 0 && (str[i] == '-' || str[i] == '+'))))
            flag = false;
    }
    return flag;
}


void checkSyntaxErrors(const char * line, size_t i, size_t lineNum)
{
    while(line[i] && (line[i] == 10 || line[i] == 9 ||line[i] == 32))
    {
        i++;
    }
//    printf("hey! %c\n", line[i]);
    if(line[i] && !(line[i] == 10 || line[i] == 9 ||line[i] == 32))
        error(E_SYNTAX, lineNum);
}


void insertData(int c)
{
    numGuidance++;
    data = (GuidanceLine *) realloc(data, numGuidance * sizeof(GuidanceLine));
    data[numGuidance - 1].code = c;
    data[numGuidance - 1].address = dc;
    dc++;
}


void dataGuidance(const char * line, size_t i, size_t lineNum)
{
    if(strlen(line) < i + 7 || strcmp("data ", str_slice(line, i + 1, i + 6)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }

    size_t j = i + 5;
    while (line[j])
    {
        size_t k = j;
        if (line[j] == ',')
            j++;
        while (line[j] && line[j++] != ',') {}

        if(line[k] != 32 && line[k] != 10)
        {
            error(E_SYNTAX, lineNum);
        }
        printf("%s\n", str_slice(line, k + 1, j - 1));
        if (j - 1 > k + 1 && isNumber(str_slice(line, k + 1, j - 1)))
        {
            insertData(atoi(str_slice(line, k + 1, j - 1)));
        }
        else
            error(E_SYNTAX, lineNum);
    }
    if(line[j] == ',')
        error(E_SYNTAX, lineNum);
}




void stringGuidance(const char * line, size_t i, size_t lineNum)
{
    size_t j = i + 11;

    if(strlen(line) < i + 10 || strcmp("string \"", str_slice(line, i + 1, i + 9)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }

    while(line[j] && line[j] != -30)
    {
        insertData(line[j]);
        j++;
    }
    if(line[j - 2] == -30)
        error(E_SYNTAX, lineNum);
    insertData('\0');
    checkSyntaxErrors(line, j - 1, lineNum);
}



void entryGuidance(const char * line, size_t i, size_t lineNum)
{
    if(strlen(line) < i + 9 || strcmp("entry ", str_slice(line, i + 1, i + 7)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }

    numEntries++;
    entrySymbols = (EntrySymbol*) realloc(entrySymbols, numEntries * sizeof(Symbol));
    size_t j = i;
    while(line[j++]){}

    if (i + 7 < j - 2)
        strcpy(entrySymbols[numEntries - 1].name, str_slice(line, i + 7, j - 2));
    checkSyntaxErrors(line, j - 1, lineNum);
}



void externalGuidance(const char * line, size_t i, size_t lineNum)
{
    cnt++;
    symbolTable = (Symbol *) realloc(symbolTable, cnt * sizeof(Symbol));
    size_t j = i;
    while(line[j++]){}
    strcpy(symbolTable[cnt - 1].symbolName, str_slice(line, i + 8, j - 2));
    symbolTable[cnt - 1].address = 0;
    symbolTable[cnt - 1].type = 1;
    symbolTable[cnt - 1].isInstruction = false;
    checkSyntaxErrors(line, j - 1, lineNum);
}




void guidanceSen(const char * line, size_t i, size_t lineNum)
{
    if (i + 2 < strlen(line))
    {
        char* s = str_slice(line, i + 1, i + 3);

        if (!strcmp(s, "da"))
        {
            dataGuidance(line, i, lineNum);
        }
        else if (!strcmp(s, "st"))
        {
            stringGuidance(line, i, lineNum);
        }

        else if (!strcmp(s, "en")){
            entryGuidance(line, i, lineNum);
        }

        else if (!strcmp(s, "ex"))
        {
            externalGuidance(line, i, lineNum);
        }
        else
            error(E_SYNTAX, lineNum);
    }
}

