//
// Created by linux on 8/10/20.
//

#include "first_transition.h"

#include "instruction_validation.h"
#include "code_func.h"
#include <stdlib.h>
#include <string.h>



size_t ic = 100;
size_t dc = 0;
size_t cnt = 0;
size_t numLines = 0;
size_t numGuidance = 0;



void checkLine(const char * line);

Symbol *symbolTable;
DirectiveLine *memory;
GuidanceLine *data;


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


char* isLabel(const char * line, size_t i) {
    size_t j = i;
    while (line[j] && (line[j] != ':'))
    {
        j++;
    }
    if(isInTable(str_slice(line, i, j)) != -1)
        return str_slice(line, j + 1, strlen(line));
    return NULL;
}



void directiveSen(const char * line, size_t i)
{
    numLines++;
    size_t j = i + 3;
    memory = (DirectiveLine*)realloc(memory, numLines * sizeof(DirectiveLine));
    strcpy(memory[numLines - 1].text, line);
    memory[numLines - 1].address = ic;
    ic++;
    if(!strcmp("stop", str_slice(line, i, i + 4)))
        return;

    while(line[j] && (line[j] == ' ' || line[j] == '\t'))
        j++;

    if (line[j] && !(line[j] == 'r' && j + 1 < strlen(line) && (line[j + 1] >= '1' && line[j + 1] <= '7')))
        ic++;

    while(line[j] && !(line[j] == ','))
        j++;

    if (line[j] && line[j] == ',')
    {
        j++;
        while(line[j] && (line[j] == ' ' || line[j] == '\t'))
            j++;

        if (line[j] && !(line[j] == 'r' && j + 1 < strlen(line) && (line[j + 1] >= '1' && line[j + 1] <= '7')))
            ic++;
    }
}




bool isGuidance(const char* line, size_t i)
{
    while(line[i] && line[i] != '.'){i++;}
    return line[i] && line[i] == '.';
}

void label(const char * line, size_t i) {
    size_t j = i;

    while (line[j] && (line[j] != ':'))
    {
        j++;
    }
    if(isInTable(str_slice(line, i, j)) == -1) {
        if (line[j] && line[j] == ':') {
            cnt++;
            symbolTable = (Symbol *) realloc(symbolTable, cnt * sizeof(Symbol));
            strcpy(symbolTable[cnt - 1].symbolName, str_slice(line, i, j));
            if (!isGuidance(line, j)) {
                symbolTable[cnt - 1].isInstruction = true;
                symbolTable[cnt - 1].address = ic;
            }
            else {
                symbolTable[cnt - 1].isInstruction = false;
                symbolTable[cnt - 1].address = dc;
            }
            symbolTable[cnt - 1].isExtern = false;
            checkLine(str_slice(line, j + 1, strlen(line)));
        }
    }
}


void guidanceSen(const char * line, size_t i)
{
    if (i + 2 < strlen(line))
    {
        char* s = str_slice(line, i + 1, i + 3);

        if (!strcmp(s, "da"))
        {
            size_t j = i + 5;
            printf("%s", line);
            while(line[j])
            {
                size_t k = j;
                if (line[j] == ',')
                    j++;
                while (line[j] && line[j++] != ',') {}
                printf("%s\n", str_slice(line, k + 1, j));
//                if (!isNumber(str_slice(line, k + 1, j)))
//                        printf("error...");
                numGuidance++;
                data = (GuidanceLine*)realloc(data, numGuidance * sizeof(GuidanceLine));
                data[numGuidance - 1].code = atoi(str_slice(line, k, j + 1));
                data[numGuidance - 1].address = dc;
                dc++;
            }

        }
        else if (!strcmp(s, "st"))
        {
            printf("%s", line);
            size_t j = i + 11;
            size_t counter = 0;
            while(line[j] && line[j] != -30)
            {
                numGuidance++;
                data = (GuidanceLine *) realloc(data, numGuidance * sizeof(GuidanceLine));
                data[numGuidance - 1].code = line[j];
                data[numGuidance - 1].address = dc;
                dc++;
                j++;
            }
            numGuidance++;
            data = (GuidanceLine *) realloc(data, numGuidance * sizeof(GuidanceLine));
            data[numGuidance - 1].code = '\0';
            data[numGuidance - 1].address = dc;
            dc++;
        }

        else if (!strcmp(s, "en")){}

        else if (!strcmp(s, "ex"))
        {
            cnt++;
            symbolTable = (Symbol *) realloc(symbolTable, cnt * sizeof(Symbol));
            size_t j = i;
            while(line[j++]){}
            strcpy(symbolTable[cnt - 1].symbolName, str_slice(line, i + 8, j - 2));
            symbolTable[cnt - 1].address = 0;
            symbolTable[cnt - 1].isExtern = true;
            symbolTable[cnt - 1].isInstruction = false;
        }
    }
}




void checkLine(const char * line)
{
    size_t i = 0;
    while(line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;

    if (line[i] == ';')
        return;

    if (line[i] == '.')
        guidanceSen(line, i);

    else if ((i + 3 < strlen(line) && line[i + 3] == ' ' && getOpcode(str_slice(line, i, i + 3)))
             || i + 4 < strlen(line) && !strcmp("stop", str_slice(line, i, i + 4)))
        directiveSen(line, i);

    else
        label(line, i);

}


void first_iteration(const char * fileName) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    fp = fopen(fileName, "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);
    memory = (DirectiveLine *)malloc(sizeof(DirectiveLine));
    data = (GuidanceLine*)malloc(sizeof(GuidanceLine));
    symbolTable = (Symbol*)malloc(sizeof(Symbol));
    while ((getline(&line, &len, fp)) != -1)
        checkLine(line);

    fclose(fp);
    if (line)
        free(line);

    printf("%d\n", ic);
    printf("%d\n", dc);

    for (int i = 0; i < cnt; ++i)
    {
        if (!symbolTable[i].isInstruction && !symbolTable[i].isExtern)
            symbolTable[i].address += ic;
        printf("%s\t%d\t%d\t%d\n", symbolTable[i].symbolName, symbolTable[i].isInstruction, symbolTable[i].address, symbolTable[i].isExtern);
        printf("%d\n", strlen(symbolTable[i].symbolName));
    }
}