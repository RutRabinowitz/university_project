
#include "dst_operand.h"
#include <stddef.h>
#include "second_transition.h"
#include "directives_table.h"
#include "read_write_files.h"
#include <stdlib.h>
#include <string.h>
#include "first_transition.h"

extern Symbol *symbolTable;
extern Directive *directive;



/* ------------------ Auxiliary Functions ---------------- */

/* The function takes a boolean value that indicates if there is another Third memory word in the directive,
 * and if so - what is the memory word. It updates the memory words and returns them. */
static Word setThirdWord(bool val, int word, Word word1)
{
    Word result = word1;
    result.isThird = val;
    result.thirdWord =  word;
    result.extSymbols[1] = -1;
    return result;
}

/* The function gets a string of a relative sentence whose addressing method of
its destination operand is a immediate addressing.
The function sets the corresponding bits in the first memory word. */
static Word relativeAddress(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    int idx;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    idx = isInTable(str_slice(line.text, k + 1, j));
    if (idx == -1 || symbolTable[idx].isExtern == 1)
        error(E_SECOND_OPERAND, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodDst[2])
    {
        setCurrWordBits(11, 12, 2);
        setCurrWordBits(8, 10, 0);
        result = setThirdWord(true, ((symbolTable[idx].address - line.address) << 3)|4, result);
        if(symbolTable[idx].isExtern == 1)
            result.extSymbols[1] = idx;
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}

/* The function gets a string of a directive sentence whose addressing method of
its destination operand is a direct addressing.
The function sets the corresponding bits in the first memory word. */
static Word directAddress(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    int idx;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    idx = isInTable(str_slice(line.text, k, j));
    if (idx == -1)
    {
        error(E_SYMBOL, line.lineNum);
    }

    else if (directives[directiveIdx].AddressingMethodDst[1])
    {
        setCurrWordBits(11, 12, 1);
        setCurrWordBits(8, 10, 0);
        result = setThirdWord(true, (symbolTable[idx].address << 3)|symbolTable[idx].isExtern, result);

        if(symbolTable[idx].isExtern == 1)
            result.extSymbols[1] = idx;
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}


/* The function gets a string of a directive sentence whose addressing method of
its destination operand is a immediate addressing.
The function sets the corresponding bits in the first memory word. */
static Word immediateAddress(DirectiveLine line, size_t j, size_t directiveIdx, Word result)
{
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    if (!isNumber(str_slice(line.text, k + 1, j)))
        error(E_SECOND_OPERAND, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodDst[0])
    {
        setCurrWordBits(8, 12, 0);
        result = setThirdWord(true, (atoi(str_slice(line.text, k + 1, j)) << 3)|4, result);
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}

/* ------------------ The main function in the file ---------------- */

/* The function takes a directive sentence that has a destination operand,
 checks what the destination operand is and what its addressing method is and sets the appropriate bits.*/
Word dstOperand(DirectiveLine line, size_t j, size_t directiveIdx, Word words_2nd_3rd)
{
    Word result = words_2nd_3rd;
    if (line.text[j] && !(line.text[j] == 'r' && j + 1 < strlen(line.text) && (line.text[j + 1] >= '1' && line.text[j + 1] <= '7'))) {
        if (line.text[j] == '&') {
            result = relativeAddress(line, j, directiveIdx, result);
        }
        else if (line.text[j] == '#')
            result = immediateAddress(line, j, directiveIdx, result);

        else
            result = directAddress(line, j, directiveIdx, result);
    }
    else if (directives[directiveIdx].AddressingMethodDst[3])
    {
        setCurrWordBits(11, 12, 3);
        setCurrWordBits(8, 10, line.text[j + 1] - 48);
        result.isThird = false;
    }
    else
        error(E_SECOND_OPERAND, line.lineNum);
    return result;
}
