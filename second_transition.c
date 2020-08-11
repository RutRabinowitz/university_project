//
// Created by linux on 8/10/20.
//

#include "second_transition.h"
#include "first_transition.h"
#include <string.h>
#include "code_func.h"
#include "instruction_validation.h"
#include <stdlib.h>


typedef struct Word{
    bool isFirst;
    int firstWord;
    bool isSecond;
    int secondWord;
} Word;

void checkLine(const char * line);
int isInTable(const char * symbol);


extern Symbol *symbolTable;
extern DirectiveLine *memory;
extern GuidanceLine *data;

extern size_t ic;
extern size_t numLines;
extern size_t numGuidance;



int a;
void printBinaryRepresentation(int number, size_t j)
{
    unsigned int musk = 1 << j - 1;
    int i;

    for (i = 0; i < j; ++i, musk = musk >> 1)
    {
        musk & number? printf("%d", 1):printf("%d", 0);
    }
}

int p(int num, int start, int stop, int number)
{
    number = number <<  start;
    for (size_t i = 0; i < 24; ++i)
        a = a | number;

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





Word relative_address(const char * line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    while (line[j] && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j++] != ',') {}
    j--;
    int idx = isInTable(str_slice(line, k, j));
    result.firstWord = symbolTable[idx].address;
//    printf("\n%d\n", symbolTable[idx].address);
    if (idx == -1)
        printf("3error...");
    else if (directives[directiveIdx].AddressingMethodSrc[1])
    {
        printBinaryRepresentation(1, 2);
        printBinaryRepresentation(0, 3);
        p(a, 16, 17, 1);
        p(a, 13, 15, 0);
        result.isFirst= true;
    }
    return result;
}

Word direct_address(const char * line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    while (line[j] && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j++] != ',') {}
    j--;
    int idx = isInTable(str_slice(line, k + 1, j));
    if (idx == -1 || symbolTable[idx].isExtern == true)
        printf("1error...");
    else if (directives[directiveIdx].AddressingMethodSrc[2]) {
        printBinaryRepresentation(2, 2);
        printBinaryRepresentation(0, 3);
        p(a, 16, 17, 2);
        p(a, 13, 15, 0);
        result.isFirst= true;
        result.firstWord = symbolTable[idx].address;
    }
    return result;
}


Word immediate_address(const char * line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    while (line[j] && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j++] != ',') {}
    j--;
    if (!isNumber(str_slice(line, k + 1, j)))
        printf("2error...");
    else if (directives[directiveIdx].AddressingMethodSrc[0]) {
        printBinaryRepresentation(0, 2);
        printBinaryRepresentation(0, 3);
        p(a, 16, 17, 0);
        p(a, 13, 15, 0);
        result.isFirst= true;
        result.firstWord= atoi(str_slice(line, k + 1, j));
    }
    return result;
}


Word first_operand(const char * line, size_t j, size_t directiveIdx)
{
    Word result;
    if (line[j] && !(line[j] == 'r' && j + 1 < strlen(line) && (line[j + 1] >= '1' && line[j + 1] <= '7'))) {
        if (line[j] == '&')
        {
            result = direct_address(line, j, directiveIdx);

        }
        else if (line[j] == '#') {
            result = immediate_address(line, j, directiveIdx);
        }
        else {
            result = relative_address(line, j, directiveIdx);
        }
    }
//    printf("%d ", directiveIdx);
    else if (directives[directiveIdx].AddressingMethodSrc[3])
    {
        printBinaryRepresentation(3, 2);
        printBinaryRepresentation(line[j + 1] - 48, 3);
        p(a, 16, 17, 3);
        p(a, 13, 15, line[j + 1] - 48);
        result.isFirst= false;
    }

    return result;
}


Word second_relative_address(const char * line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line[j] && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j++] != ',') {}
    int idx = isInTable(str_slice(line, k + 1, j));

    if (idx == -1 || symbolTable[idx].isExtern == true)
        printf("error...");
    else if (directives[directiveIdx].AddressingMethodDst[2]) {
        printBinaryRepresentation(2, 2);
        printBinaryRepresentation(0, 3);

        p(a, 11, 12, 2);
        p(a, 8, 10, 0);
        result.isSecond= true;
        result.secondWord = symbolTable[idx].address;
    }
    return result;
}


Word second_direct_address(const char * line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line[j] && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j++] != ',') {}
    int idx = isInTable(str_slice(line, k, j));
    if (idx == -1)
        printf("error...");
    else if (directives[directiveIdx].AddressingMethodDst[1]) {
        printBinaryRepresentation(1, 2);
        printBinaryRepresentation(0, 3);
        p(a, 11, 12, 1);
        p(a, 8, 10, 0);
        result.isSecond= true;
        result.secondWord = symbolTable[idx].address;
    }
    return result;
}

Word second_immediate_address(const char * line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line[j] && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j++] != ',') {}
    if (!isNumber(str_slice(line, k + 1, j)))
        printf("error...");
    else if (directives[directiveIdx].AddressingMethodDst[0]) {
        printBinaryRepresentation(0, 5);
        p(a, 8, 12, 0);
        result.isSecond = true;
        result.secondWord = atoi(str_slice(line, k + 1, j));
    }
    return result;
}

Word second_operand(const char * line, size_t j, size_t directiveIdx, Word result)
{
    if (line[j] && !(line[j] == 'r' && j + 1 < strlen(line) && (line[j + 1] >= '1' && line[j + 1] <= '7'))) {
        if (line[j] == '&') {
            result = second_relative_address(line, j, directiveIdx, result);
        }
        else if (line[j] == '#')
        {
            result = second_immediate_address(line, j, directiveIdx, result);
        }
        else
            {
            result = second_direct_address(line, j, directiveIdx, result);
        }
    }
    else if (directives[directiveIdx].AddressingMethodDst[3])
    {
        printBinaryRepresentation(3, 2);
        printBinaryRepresentation(line[j + 1] - 48, 3);
        p(a, 11, 12, 3);
        p(a, 8, 10, line[j + 1] - 48);
        result.isSecond = false;
    }
    return result;
}



Word codeDirective(const char * line, size_t i, size_t num)
{
    a = 0;
    printf("%s", line);
    printf("0000");
    printf("%d\t", num++);
    Word result;
    if(!strcmp("stop", str_slice(line, i, i + 4)))
    {
        printf("001111000000000000000100");
        return result;
    }
//    printf("%s\n", str_slice(line,i, i + 3));
    int directiveIdx = getOpcod(str_slice(line,i, i + 3));
    p(a, 18, 23, directives[directiveIdx].opcode)  ;
    printBinaryRepresentation(directives[directiveIdx].opcode, 6);
    size_t j = i + 3;

    while(line[j] && (line[j] == ' ' || line[j] == '\t'))
        j++;

    if(isTwoOperands(line))
    {
        if(isValidTwoOperands(directiveIdx))
        {
            result = first_operand(line, j, directiveIdx);
            while(line[j] && !(line[j] == ','))
                j++;
            if (j + 1 < strlen(line) && line[j + 1] == ' ')
                while(line[++j] && (line[j] == ' ' || line[j] == '\t')){}
        }
        else
            {
            printf("error... extra operand");
        }
    }
    else
    {
        result.isFirst = false;
        p(a, 13, 17, 0);
        printBinaryRepresentation(0, 5);
    }
    result = second_operand(line, j, directiveIdx, result);
    printBinaryRepresentation(directives[directiveIdx].funct, 5);
    printBinaryRepresentation(4, 3);
    p(a, 3, 7, directives[directiveIdx].funct);
    p(a, 0, 2, 4);
    printf("\n");
//    printf("%x\n", a);
    if(result.isFirst)
    {
        a = 0;
        printf("0000");
        printf("%d\t", num++);
        printBinaryRepresentation(result.firstWord, 21);
        printBinaryRepresentation(4, 3);
        p(a, 3, 23, result.firstWord);
        p(a, 0, 2, 4);
//        printf("\n%x", a);
        printf("\n");
    }
    if(result.isSecond)
    {
        a = 0;
        printf("0000");
        printf("%d\t", num);
        printBinaryRepresentation(result.secondWord, 21);
        printBinaryRepresentation(4, 3);
        p(a, 3, 23, result.secondWord);
        p(a, 0, 2, 4);
//        printf("\n%x", a);
        printf("\n");
    }
    return result;
}

void codeLine(const char * line, size_t num)
{

    size_t i = 0;
    while(line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;

    if (line[i] == ';')
        return;

    if (line[i] == '.')
        return;
    else if ((i + 3 < strlen(line) && line[i + 3] == ' ' && getOpcode(str_slice(line, i, i + 3)) !=-1)
             || i + 4 < strlen(line) && !strcmp("stop", str_slice(line, i, i + 4)))
    {
        codeDirective(line, i, num);
    }
//TODO: if needed
//    else{
//        if (isLabel(line, i) == -1)
//            printf("\nError...");
//        else
//        {
//            codeLine(isLabel(line, i), num);
//        }
//    }
}

void codeGuidance(GuidanceLine line)
{
    printf("0000");
    printf("%d\t", line.address + ic);
    printBinaryRepresentation(line.code, 24);
    printf("\n");
    printf("%x\n", line.code);
}

void second_iteration(const char * fileName)
{
    for(size_t i = 0; i < numLines; ++i)
        codeLine(memory[i].text, memory[i].address);
    printf("\n");
    for(size_t i = 0; i < numGuidance; ++i)
        codeGuidance(data[i]);
}

