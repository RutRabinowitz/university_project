//
// Created by linux on 8/10/20.
//

#include "second_transition.h"
#include <string.h>
#include "code_func.h"
#include "first_operand.h"
#include "second_operand.h"
#include "read_files.h"
#include <stdlib.h>


//int isInTable(const char * symbol);


extern Symbol *symbolTable;
extern DirectiveLine *memory;
extern GuidanceLine *data;

extern size_t cnt;
extern size_t ic;
extern size_t numLines;
extern size_t numGuidance;
extern size_t dc;
int currentWord;
int *output;



int setCurrWordBits(int start, int stop, int number)
{
    number = number <<  start;
    for (size_t i = 0; i < 24; ++i)
        currentWord = currentWord | number;

}



bool isValidTwoOperands(size_t idx)
{
    size_t i;
    for(i = 0; i < 4; ++i)
    {
        if (directives[idx].AddressingMethodSrc[i])
            return true;
    }
    return false;
}



bool isTwoOperands(const char * line)
{
    size_t i, len = strlen(line);
    for (i = 0; i < len; ++i)
    {
        if (line[i] == ',')
            return true;
    }
    return false;
}






void printFirst(Word word, size_t num)
{
    if(word.isFirst)
    {
        output[num - 99] = word.firstWord;
    }
}


void printSecond(Word word, size_t num)
{
    if(word.isSecond)
    {
        currentWord = 0;
        if (word.isFirst)
            output[num - 98] = word.secondWord;
        else
            output[num - 99] = word.secondWord;
    }
}


void func()
{
    if (currentWord == 14)
        currentWord = setCurrWordBits(0, 24, 3932164);
    else
        currentWord = setCurrWordBits(0, 24, 14680068);
}


Word codeDirective(DirectiveLine line, size_t i)
{
    currentWord = 0;
    Word result;

    int directiveIdx = getOpcode(str_slice(line.text,i, i + 3));
    if (directiveIdx == 14 || (strlen(line.text) >= 4 && !strcmp(str_slice(line.text, i, i + 4), "stop")))
    {
        func();
        return result;
    }
    else{
        setCurrWordBits(18, 23, directives[directiveIdx].opcode);
        size_t j = i + 3;

        while(line.text[j] && (line.text[j] == ' ' || line.text[j] == '\t'))
            j++;

        if(isTwoOperands(line.text))
        {
            if(isValidTwoOperands(directiveIdx))
            {

                result = first_operand(line, j, directiveIdx);
                while(line.text[j] && !(line.text[j] == ','))
                    j++;
                if (j + 1 < strlen(line.text) && line.text[j + 1] == ' ')
                    while(line.text[++j] && (line.text[j] == ' ' || line.text[j] == '\t')){}
                else
                    error(E_SYNTAX, line.lineNum);
            }
            else
            {
                error(E_EXTRA_OPERAND, line.lineNum);
                return result;
            }
        }
        else
        {
            result.isFirst = false;
            setCurrWordBits( 13, 17, 0);
        }

        Word result2 = second_operand(line, j, directiveIdx, result);
        result.isSecond = result2.isSecond;
        result.secondWord = result2.secondWord;
        setCurrWordBits(3, 7, directives[directiveIdx].funct);
        setCurrWordBits(0, 2, 4);
    }
    output[line.address - 100] = currentWord;
    printFirst(result, line.address);
    printSecond(result, line.address);
    return result;
}

void codeLine(DirectiveLine line)
{
    size_t i = 0;
    while(line.text[i] && (line.text[i] == ' ' || line.text[i] == '\t'))
        i++;

    if ((i + 3 < strlen(line.text) && line.text[i + 3] == ' ' && getOpcode(str_slice(line.text, i, i + 3)) != -1)
             || i + 4 < strlen(line.text) && !strcmp("stop", str_slice(line.text, i, i + 4)))
    {
        codeDirective(line, i);
    }
    else
        error(E_SYNTAX, line.lineNum);
}


void codeGuidance(GuidanceLine line)
{
    output[line.address + ic - 100] = line.code;
}


void freeTables()
{
    free(symbolTable);
    free(memory);
    free(data);
}

int* second_iteration(const char * fileName)
{
    output = (int*)malloc((dc + ic - 100)*sizeof(int));
    for (int i = 0; i < cnt; ++i)
    {
        printf("%d: %s\t%d\t%ld\t%d\n", i, symbolTable[i].symbolName, symbolTable[i].isInstruction, symbolTable[i].address, symbolTable[i].type);
    }
    for(size_t i = 0; i < numLines; ++i)
        codeLine(memory[i]);
    printf("\n");
    for(size_t i = 0; i < numGuidance; ++i)
        codeGuidance(data[i]);
    freeTables();
    return output;
}

