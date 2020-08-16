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

void insertData(int c)
{
    numGuidance++;
    data = (GuidanceLine *) realloc(data, numGuidance * sizeof(GuidanceLine));
    data[numGuidance - 1].code = c;
    data[numGuidance - 1].address = dc;
    dc++;
}

void dataGuidance(const char * line, size_t i) {

    size_t j = i + 5;
    while (line[j])
    {
        size_t k = j;
        if (line[j] == ',')
            j++;
        while (line[j] && line[j++] != ',') {}
        insertData(atoi(str_slice(line, k, j + 1)));
    }

}




void stringGuidance(const char * line, size_t i)
{
    size_t j = i + 11;
    while(line[j] && line[j] != -30)
    {
        insertData(line[j]);
        j++;
    }
    insertData('\0');
}



void entryGuidance(const char * line, size_t i)
{
    numEntries++;
    entrySymbols = (EntrySymbol*) realloc(entrySymbols, numEntries * sizeof(Symbol));
    size_t j = i;
    while(line[j++]){}
    strcpy(entrySymbols[numEntries - 1].name, str_slice(line, i + 7, j - 2));
}



void externalGuidance(const char * line, size_t i)
{
    cnt++;
    symbolTable = (Symbol *) realloc(symbolTable, cnt * sizeof(Symbol));
    size_t j = i;
    while(line[j++]){}
    strcpy(symbolTable[cnt - 1].symbolName, str_slice(line, i + 8, j - 2));
    symbolTable[cnt - 1].address = 0;
    symbolTable[cnt - 1].type = 1;
    symbolTable[cnt - 1].isInstruction = false;
}




void guidanceSen(const char * line, size_t i, size_t lineNum)
{
    if (i + 2 < strlen(line))
    {
        char* s = str_slice(line, i + 1, i + 3);

        if (!strcmp(s, "da"))
        {
            dataGuidance(line, i);
        }
        else if (!strcmp(s, "st"))
        {
            stringGuidance(line, i);
        }

        else if (!strcmp(s, "en")){
            entryGuidance(line, i);
        }

        else if (!strcmp(s, "ex"))
        {
            externalGuidance(line, i);
        }
        else
            error(E_SYNTAX, lineNum);
    }
}

