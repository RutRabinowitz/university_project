
#include "second_transition.h"
#include <string.h>
#include "code_func.h"
#include "src_operand.h"
#include "dst_operand.h"
#include "read_files.h"
#include <stdlib.h>


#define SPC 32
#define TAB 9
#define NEW_LINE 10

extern Symbol *symbolTable;
extern DirectiveLine *memory;
extern GuidanceLine *data;

extern EntrySymbol* entrySymbols;

extern size_t cnt;
extern size_t ic;
extern size_t numLines;
extern size_t numGuidance;
extern size_t dc;
int currentWord;
int *output;

EntrySymbol* externs;
size_t numExterns= 0;

void setCurrWordBits(int start, int stop, int number)
{
    size_t i;
    number = number <<  start;
    for (i = 0; i < 24; ++i)
        currentWord = currentWord | number;
}


static bool isValidTwoOperands(size_t idx)
{
    size_t i;
    for(i = 0; i < ADDRESS_MTD_NUM; ++i)
    {
        if (directives[idx].AddressingMethodSrc[i])
            return true;
    }
    return false;
}


static bool isTwoOperands(const char * line)
{
    size_t i, len = strlen(line);

    for (i = 0; i < len; ++i)
    {
        if (line[i] == ',')
            return true;
    }
    return false;
}


static bool isNoValidOperands(size_t idx)
{
    size_t i;
    for(i = 0; i < ADDRESS_MTD_NUM; ++i)
    {
        if (directives[idx].AddressingMethodSrc[i])
            return false;
        if (directives[idx].AddressingMethodDst[i])
            return false;
    }
    return true;
}


static void setEntry(Word word, size_t num, DirectiveLine line)
{

    if(word.extSymbols[0] != -1)
    {
        numExterns++;
        externs = (EntrySymbol*)realloc(externs, numExterns * sizeof(EntrySymbol));
        strcpy(externs[numExterns - 1].name, symbolTable[word.extSymbols[0]].symbolName);
        externs[numExterns - 1].numLine = num - (START - 1);
    }

    if(word.extSymbols[1] != -1)
    {
        numExterns++;
        externs = (EntrySymbol *) realloc(externs, numExterns * sizeof(EntrySymbol));
        strcpy(externs[numExterns - 1].name, symbolTable[word.extSymbols[1]].symbolName);
        if (word.extSymbols[0] != -1)
            externs[numExterns - 1].numLine = num - (START - 2);
        else
            externs[numExterns - 1].numLine = num - (START - 1);
    }
}


static void insertSecondWord(Word word, size_t num)
{
    if(word.isFirst)
    {
        output[num - (START - 1)] = word.firstWord;
    }
}


static void insertThirdWord(Word word, size_t num)
{
    if(word.isSecond)
    {
        if (word.isFirst)
            output[num - (START - 2)] = word.secondWord;
        else
            output[num - (START - 1)] = word.secondWord;
    }
}



static void checkExtraOperandErrors(const char * line, size_t i, size_t lineNum)
{
    while(line[i] && (line[i] == NEW_LINE || line[i] == TAB ||line[i] == SPC))
    {
        i++;
    }
    if(line[i] && !(line[i] == NEW_LINE || line[i] == TAB ||line[i] == SPC))
        error(E_EXTRA_OPERAND, lineNum);

}



static void oneWordDirectives(DirectiveLine line, size_t idx)
{
    setCurrWordBits(18, 23, directives[idx].opcode);
    setCurrWordBits(0, 2, 4);
    output[line.address - START] = currentWord;
    checkExtraOperandErrors(line.text, 5, line.lineNum);
}


static Word initWord()
{
    Word result;
    result.isFirst = result.isSecond = false;
    result.firstWord = result.secondWord = 0;
    result.extSymbols[0] = result.extSymbols[1] = -1;
    return result;
}


static Word twoOperand(DirectiveLine line, size_t j, int directiveIdx)
{
    Word result = initWord();
    if(isValidTwoOperands(directiveIdx))
    {
        result = srcOperand(line, j, directiveIdx);
    }
    else
    {
        error(E_EXTRA_OPERAND, line.lineNum);
        return result;
    }
    return result;
}


static size_t updateIdx(DirectiveLine line, size_t j)
{
    while(line.text[j] && !(line.text[j] == ','))
        j++;
    if (j + 1 < strlen(line.text) && line.text[j + 1] == ' ')
        while(line.text[++j] && (line.text[j] == ' ' || line.text[j] == '\t')){}
    else
        error(E_SYNTAX, line.lineNum);
    return j;
}


void insertMemoryWordToInput(DirectiveLine line, Word words_2nd_3rd)
{
    output[line.address - START] = currentWord;
    insertSecondWord(words_2nd_3rd, line.address);
    insertThirdWord(words_2nd_3rd, line.address);
    setEntry(words_2nd_3rd, line.address, line);
}


static void codeDirective(DirectiveLine line, size_t i)
{
    size_t j;
    Word words_2nd_3rd = initWord();
    int directiveIdx = getOpcode(str_slice(line.text,i, i + 3));;
    currentWord = 0;

    if (isNoValidOperands(directiveIdx) || !strcmp(str_slice(line.text,i, i + 4), "stop"))
        oneWordDirectives(line, directiveIdx);
    else
        {
        setCurrWordBits(18, 23, directives[directiveIdx].opcode);
        setCurrWordBits(3, 7, directives[directiveIdx].funct);
        setCurrWordBits(0, 2, 4);
        j = i + 3;
        while(line.text[j] && (line.text[j] == ' ' || line.text[j] == '\t'))
            j++;

        if(isTwoOperands(line.text))
        {
            words_2nd_3rd = twoOperand(line, j, directiveIdx);
            j = updateIdx(line, j);
        }
        else
        {
            words_2nd_3rd.isFirst = false;
            setCurrWordBits( 13, 17, 0);
        }
        words_2nd_3rd = dstOperand(line, j, directiveIdx, words_2nd_3rd);

    }
    insertMemoryWordToInput(line, words_2nd_3rd);
}


static void codeLine(DirectiveLine line)
{

    size_t i = 0;
    while(line.text[i] && (line.text[i] == ' ' || line.text[i] == '\t'))
        i++;

    if ((i + 3 < strlen(line.text) && line.text[i + 3] == ' ' && getOpcode(str_slice(line.text, i, i + 3)) != -1)
             || (i + 4 < strlen(line.text) && !strcmp("stop", str_slice(line.text, i, i + 4))))
    {
        codeDirective(line, i);
    }
    else
        error(E_SYNTAX, line.lineNum);
}


static void codeGuidance(GuidanceLine line)
{
    output[line.address + ic - START] = line.code;
}


static void freeTables()
{
    free(memory);
    free(data);
}


/*The function makes the second transition on the input:
It encodes the directive sentences and guidance sentences according to the rules
 and returns an array of encoding memory words*/
int* secondIteration()
{
    size_t i;
    output = (int*)malloc((dc + ic - START)*sizeof(int));
    externs = (EntrySymbol*)malloc(sizeof(EntrySymbol));
    for(i = 0; i < numLines; ++i)
        codeLine(memory[i]);
    for(i = 0; i < numGuidance; ++i)
        codeGuidance(data[i]);
    freeTables();
    return output;
}

