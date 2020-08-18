//
// Created by linux on 8/12/20.
//

#include "second_operand.h"
#include <stddef.h>
#include "second_transition.h"
#include "code_func.h"
#include "read_files.h"
#include <stdlib.h>
#include <string.h>
#include "first_transition.h"

extern Symbol *symbolTable;
//extern int currentWord;
extern Directive *directive;


static Word setSecondWord(bool val, int word)
{
    Word result;
    result.isSecond= val;
    result.secondWord =  word;
    return result;
}


static Word relative_address(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    int idx = isInTable(str_slice(line.text, k + 1, j));
    if (idx == -1 || symbolTable[idx].type == 1)
        error(E_SECOND_OPERAND, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodDst[2]) {
        setCurrWordBits(11, 12, 2);
        setCurrWordBits(8, 10, 0);
        result = setSecondWord(true, (symbolTable[idx].address - line.address << 3)|4);
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}


static Word direct_address(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    int idx = isInTable(str_slice(line.text, k, j));
    if (idx == -1)
    {
        error(E_SYMBOL, line.lineNum);
    }

    else if (directives[directiveIdx].AddressingMethodDst[1])
    {
        setCurrWordBits(11, 12, 1);
        setCurrWordBits(8, 10, 0);
        result = setSecondWord(true, (symbolTable[idx].address << 3)|symbolTable[idx].type);

    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}


static Word immediate_address(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    if (!isNumber(str_slice(line.text, k + 1, j)))
        error(E_SECOND_OPERAND, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodDst[0])
    {
        setCurrWordBits(8, 12, 0);
        result = setSecondWord(true, (atoi(str_slice(line.text, k + 1, j)) << 3)|4);
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}


Word second_operand(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    if (line.text[j] && !(line.text[j] == 'r' && j + 1 < strlen(line.text) && (line.text[j + 1] >= '1' && line.text[j + 1] <= '7'))) {
        if (line.text[j] == '&') {
            result = relative_address(line, j, directiveIdx, result);
        }
        else if (line.text[j] == '#')
        {
            result = immediate_address(line, j, directiveIdx, result);
        }
        else
        {
            result = direct_address(line, j, directiveIdx, result);
        }
    }
    else if (directives[directiveIdx].AddressingMethodDst[3])
    {
        setCurrWordBits(11, 12, 3);
        setCurrWordBits(8, 10, line.text[j + 1] - 48);
        result.isSecond = false;
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}
