#include "src_operand.h"
#include "code_func.h"
#include "second_transition.h"
#include "read_files.h"
#include <stdlib.h>
#include <string.h>


extern Directive *directive;
extern Symbol *symbolTable;
extern int cnt;


extern EntrySymbol* entrySymbols;
extern size_t numEntries;


static Word setFirstWord(bool val, int word)
{
    Word result;
    result.isFirst = val;
    result.firstWord =  word;
    result.extSymbols[0] = result.extSymbols[1] = -1;
    return result;
}


static Word initWord()
{
    Word result;
    result.isFirst = result.isSecond = false;
    result.firstWord = result.secondWord = 0;
    result.extSymbols[0] = result.extSymbols[1] = -1;
    return result;
}


/* The function gets a string of a directive sentence whose addressing method of
its first operand is a direct addressing.
The function sets the corresponding bits in the first memory word. */
static Word directAddress(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result = initWord();
    size_t k = j;
    int idx;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    j--;
    idx = isInTable(str_slice(line.text, k, j));
    if (idx == -1)
        error(E_SYMBOL, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodSrc[1]) {
        setCurrWordBits(16, 17, 1);
        setCurrWordBits(13, 15, 0);
        result = setFirstWord(true, (symbolTable[idx].address << 3)|symbolTable[idx].type);

        if(symbolTable[idx].type == 1)
            result.extSymbols[0] = idx;
    }
    else
        error(E_FIRST_OPERAND, line.lineNum);
    return result;
}


/* The function gets a string of a directive sentence whose addressing method of
its first operand is a relative addressing.
The function sets the corresponding bits in the first memory word. */
static Word relativeAddress(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    int idx;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    j--;
    idx = isInTable(str_slice(line.text, k, j));

    if (idx == -1 || symbolTable[idx].type == 1)
        error(E_FIRST_OPERAND, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodSrc[2])
    {
        setCurrWordBits(16, 17, 1);
        setCurrWordBits(13, 15, 0);
        result = setFirstWord(true, (symbolTable[idx].address - (line.address << 3))|4);
        if(symbolTable[idx].type == 1)
            result.extSymbols[0] = idx;
    }
    else
        error(E_FIRST_OPERAND, line.lineNum);
    return result;

}

/* The function gets a string of directive sentence whose addressing method of
its first operand is a immediate addressing.
The function sets the corresponding bits in the first memory word. */
static Word immediateAddress(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result;
    size_t k = j;
    while (line.text[j] && line.text[j] != ' ' && line.text[j] != '\t' && line.text[j] != '\n' && line.text[j++] != ',') {}
    j--;
    if (!isNumber(str_slice(line.text, k + 1, j)))
        error(E_FIRST_OPERAND, line.lineNum);
    else if (directives[directiveIdx].AddressingMethodSrc[0])
    {
        setCurrWordBits(16, 17, 0);
        setCurrWordBits(13, 15, 0);
        result = setFirstWord(true, (atoi(str_slice(line.text, k + 1, j)) << 3)|4);
    }
    else
        error(E_FIRST_OPERAND, line.lineNum);
    return result;
}


/* The function gets a directive sentence that has a source operand,
 checks what the source operand is and what its addressing method is and sets the appropriate bits.*/
Word  srcOperand(DirectiveLine line, size_t j, size_t directiveIdx)
{
    Word result = initWord();
    if (line.text[j] && !(line.text[j] == 'r' && j + 1 < strlen(line.text) && (line.text[j + 1] >= '1' && line.text[j + 1] <= '7'))) {
        if (line.text[j] == '&')
        {
            result = relativeAddress(line, j, directiveIdx);

        }
        else if (line.text[j] == '#') {
            result = immediateAddress(line, j, directiveIdx);
        }
        else
        {
            result = directAddress(line, j, directiveIdx);
        }
    }
    else if (directives[directiveIdx].AddressingMethodSrc[3])
    {
        setCurrWordBits(16, 17, 3);
        setCurrWordBits(13, 15, line.text[j + 1] - 48);
        result = setFirstWord(0, -1);
    }
    else
        error(E_FIRST_OPERAND, line.lineNum);
    result.extSymbols[1] = -1;
    return result;
}

