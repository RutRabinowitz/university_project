//
// Created by linux on 8/10/20.
//

#include "second_transition.h"
#include <string.h>
#include "code_func.h"
#include "instruction_validation.h"
#include "first_operand.h"
#include "second_operand.h"
#include "read_files.h"
#include <stdlib.h>




void checkLine(const char * line);
int isInTable(const char * symbol);


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



void printBinaryRepresentation(int number, size_t j)
{
    unsigned int musk = 1 << j - 1;
    int i;

    for (i = 0; i < j; ++i, musk = musk >> 1)
    {
        musk & number? printf("%d", 1):printf("%d", 0);
    }
}



void printHexNum(int word)
{
    int musk = 15 << 20;

    for (size_t i = 0; i < 6; i++, musk = musk >> 4)
    {
        printf("%x", (word&musk)>>(20 - 4*i));
    }
    printf("\n");
}

int setCurrWordBits(int start, int stop, int number)
{
    number = number <<  start;
    for (size_t i = 0; i < 24; ++i)
        currentWord = currentWord | number;

}


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



void printLineNum(int num)
{
    printf("0000");
    printf("%d\t", num);
}




void printFirst(Word word, size_t num)
{
    if(word.isFirst)
    {
        currentWord = 0;
        printLineNum(num + 1);
        printBinaryRepresentation(word.firstWord, 24);
        printf("\n");
        printHexNum(word.firstWord);
        output = (int *) realloc(output, num + 1 * sizeof(int));
        output[num] = word.firstWord;
    }
}

void printSecond(Word word, size_t num)
{
    if(word.isSecond)
    {
//        currentWord = 0;
//        printLineNum(num + 2);
//        printBinaryRepresentation(word.secondWord, 24);
//        printf("\n");
//        printHexNum(word.secondWord);
        output = (int *) realloc(output, num + 2 * sizeof(int));
        output[num + 1] = word.secondWord;
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
    printf("%s", line.text);
    printLineNum(line.address);
    Word result;

    int directiveIdx = getOpcod(str_slice(line.text,i, i + 3));
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
            }
            else
            {
                printf("error... extra operand");
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
    printBinaryRepresentation(currentWord, 24);
//    output = (int *) realloc(output, num * sizeof(int));
    output[line.address - 1] = currentWord;
//    printf("\n");
//    printHexNum(currentWord);
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
    printf("0000");
    printf("%ld\t", line.address + ic);
    printBinaryRepresentation(line.code, 24);
    printf("\n");
    printHexNum(line.code);
}


void second_iteration(const char * fileName)
{
    output = (int*)malloc((dc + ic)*sizeof(int));
    for (int i = 0; i < cnt; ++i)
    {
        printf("%ld: %s\t%d\t%ld\t%ld\n", i, symbolTable[i].symbolName, symbolTable[i].isInstruction, symbolTable[i].address, symbolTable[i].type);
    }
    for(size_t i = 0; i < numLines; ++i)
        codeLine(memory[i]);
    printf("\n");
    for(size_t i = 0; i < numGuidance; ++i)
        codeGuidance(data[i]);
    free(symbolTable);
    free(memory);
    free(data);

}

