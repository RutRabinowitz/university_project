//
// Created by linux on 8/12/20.
//

#include "first_operand.h"
#include "code_func.h"
#include "second_transition.h"
#include "instruction_validation.h"

extern Directive *directive;
extern Symbol *symbolTable;
extern int cnt;

//int isInTable(const char * symbol)
//{
//    size_t i;
//    for(i = 0; i < cnt; i++)
//    {
//        if (!strcmp(symbolTable[i].symbolName, symbol))
//        {
//            return i;
//        }
//
//    }
//    return -1;
//}

static Word setFirstWord(bool val, int word)
{
    Word result;
    result.isFirst = val;
    result.firstWord =  word;
    return result;
}

static Word direct_address(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;

    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    j--;
    int idx = isInTable(str_slice(line.text, k, j));
    if (idx == -1)
        printf("1error...");
    else if (directives[directiveIdx].AddressingMethodSrc[1]) {
        setCurrWordBits(16, 17, 1);
        setCurrWordBits(13, 15, 0);
        result = setFirstWord(true, (symbolTable[idx].address << 3)|symbolTable[idx].type);
    }
    return result;
}

static Word relative_address(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    j--;
    int idx = isInTable(str_slice(line.text, k, j));

    if (idx == -1 || symbolTable[idx].type == 1)
        printf("3error...");
    else if (directives[directiveIdx].AddressingMethodSrc[2])
    {
        setCurrWordBits(16, 17, 1);
        setCurrWordBits(13, 15, 0);
        result = setFirstWord(true, (symbolTable[idx].address - line.address << 3)|4);
    }
    return result;

}


static Word immediate_address(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    j--;
    if (!isNumber(str_slice(line.text, k + 1, j)))
        printf("2error...");
    else if (directives[directiveIdx].AddressingMethodSrc[0])
    {
        setCurrWordBits(16, 17, 0);
        setCurrWordBits(13, 15, 0);
        result = setFirstWord(true, (atoi(str_slice(line.text, k + 1, j)) << 3)|4);
    }
    return result;
}


Word first_operand(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result;
    if (line.text[j] && !(line.text[j] == 'r' && j + 1 < strlen(line.text) && (line.text[j + 1] >= '1' && line.text[j + 1] <= '7'))) {
        if (line.text[j] == '&')
        {
            result = relative_address(line, j, directiveIdx);

        }
        else if (line.text[j] == '#') {
            result = immediate_address(line, j, directiveIdx);
        }
        else
        {
            result = direct_address(line, j, directiveIdx);
        }
    }
    else if (directives[directiveIdx].AddressingMethodSrc[3])
    {
        setCurrWordBits(16, 17, 3);
        setCurrWordBits(13, 15, line.text[j + 1] - 48);
        result = setFirstWord(0, -1);
    }
    return result;
}

