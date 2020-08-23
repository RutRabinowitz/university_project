
#include "second_transition.h"
#include <string.h>
#include "directives_table.h"
#include "src_operand.h"
#include "dst_operand.h"
#include "read_write_files.h"
#include <stdlib.h>


#define SPC 32
#define TAB 9
#define NEW_LINE 10

extern Symbol *symbolTable;
extern DirectiveLine *memory;
extern GuidanceLine *data;

extern ExternSymbol * entrySymbols;
extern size_t ic;
extern size_t numLines;
extern size_t numGuidance;
extern size_t dc;
int currentWord;
int *output;

ExternSymbol* externs;
size_t numExterns;



/* ---------------- Auxiliary functions ---------------- */


/* The function takes an index of a directive in the directive table,
   and returns a boolean value that indicates whether two operands are allowed in this directive. */
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


/* The function takes a string of a line of a directive sentence,
   and returns a boolean value whether or not the sentence has two operands. */
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


/* The function takes an index of a directive in the directive table,
   and returns a boolean value that indicates whether two operands are not allowed in this directive. */
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


/* The function takes the additional memory words of the directive and the directive number,
 * and checks if they were addresses of external symbol,
 * if so, it inserts it into the externs table the name of the symbol and the directive number. */
static void setExtern(Word word, size_t num)
{
    if(word.extSymbols[0] != -1)
    {
        numExterns++;
        externs = (ExternSymbol *)realloc(externs, numExterns * sizeof(ExternSymbol));
        strcpy(externs[numExterns - 1].name, symbolTable[word.extSymbols[0]].symbolName);
        externs[numExterns - 1].numLine = num - (START - 1);
    }

    if(word.extSymbols[1] != -1)
    {
        numExterns++;
        externs = (ExternSymbol *) realloc(externs, numExterns * sizeof(ExternSymbol));
        strcpy(externs[numExterns - 1].name, symbolTable[word.extSymbols[1]].symbolName);
        if (word.extSymbols[0] != -1)
            externs[numExterns - 1].numLine = num - (START - 2);
        else
            externs[numExterns - 1].numLine = num - (START - 1);
    }
}


/* The function takes the additional memory words of the directive.
 * If there is second memory word - it inserts it into the output array. */
static void insertSecondWord(Word word, size_t num)
{
    if(word.isSecond)
    {
        output[num - (START - 1)] = word.secondWord;
    }
}


/* The function takes the additional memory words of the directive.
 * If there is third memory word - it inserts it into the output array. */
static void insertThirdWord(Word word, size_t num)
{
    if(word.isThird)
    {
        if (word.isSecond)
            output[num - (START - 2)] = word.thirdWord;
        else
            output[num - (START - 1)] = word.thirdWord;
    }
}


/* The function takes a string of directive line, index and line number,
 * and checks if from the given index onwards there are operands - if so,
 * it is invalid and it will output an error. */
static void checkExtraOperandErrors(const char * line, size_t i, size_t lineNum)
{
    while(line[i] && (line[i] == NEW_LINE || line[i] == TAB ||line[i] == SPC))
    {
        i++;
    }
    if(line[i] && !(line[i] == NEW_LINE || line[i] == TAB ||line[i] == SPC))
        error(E_EXTRA_OPERAND, lineNum);
}


/* The function takes a line of directive sentence that has one memory word without operands.
 * it inserts it into the output. */
static void oneWordDirectives(DirectiveLine line, size_t i)
{
    if (!strcmp(str_slice(line.text,i, i + 4), "stop"))
        setCurrWordBits(18, 23, 15);
    else
        setCurrWordBits(18, 23, 14);
    setCurrWordBits(0, 2, 4);
    output[line.address - START] = currentWord;
    checkExtraOperandErrors(line.text, 5, line.lineNum);
}


/* The function initializes a word instance and returns it. */
static Word initWord()
{
    Word result;
    result.isSecond = result.isThird = false;
    result.secondWord = result.thirdWord = 0;
    result.extSymbols[0] = result.extSymbols[1] = -1;
    return result;
}


/* The function takes a line of directive sentences with two operands (operand source and operand).
 * It checks the source operand and updates the memory words it returns. */
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


/* A function takes a line of directive sentence with two operands.
 * It takes the index where the first operand ends, and returns the index where the second operand begins. */
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


/* A function takes a line of directive and its memory words.
 * It updates the output array with the memory words of that directive. */
void insertMemoryWordToInput(DirectiveLine line, Word words_2nd_3rd)
{
    output[line.address - START] = currentWord;
    insertSecondWord(words_2nd_3rd, line.address);
    insertThirdWord(words_2nd_3rd, line.address);
    setExtern(words_2nd_3rd, line.address);
}


/* The function takes a line of directive sentence, and an index where the directive begins.
 * It encodes the directive into several memory words - from one memory word to three.
 * It adds to the output array the memory words of the directive. */
static void codeDirective(DirectiveLine line, size_t i)
{
    size_t j;
    Word words_2nd_3rd = initWord();
    int directiveIdx = getDirIdx(str_slice(line.text,i, i + 3));
    currentWord = 0;

    if (isNoValidOperands(directiveIdx) || !strcmp(str_slice(line.text,i, i + 4), "stop"))
        oneWordDirectives(line, i);
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
            words_2nd_3rd.isSecond = false;
            setCurrWordBits( 13, 17, 0);
        }
        words_2nd_3rd = dstOperand(line, j, directiveIdx, words_2nd_3rd);
    }
    insertMemoryWordToInput(line, words_2nd_3rd);
}


/* The function takes a line that is a directive sentence, or is invalid.
 * If it is indeed a directive line - it encodes the directive, otherwise it executes an error. */
static void codeDirectiveLine(DirectiveLine line)
{
    size_t i = 0;
    while(line.text[i] && (line.text[i] == ' ' || line.text[i] == '\t'))
        i++;

    if ((i + 3 < strlen(line.text) && line.text[i + 3] == ' ' && getDirIdx(str_slice(line.text, i, i + 3)) != -1)
             || (i + 4 < strlen(line.text) && !strcmp("stop", str_slice(line.text, i, i + 4))))
    {
        codeDirective(line, i);
    }
    else
        error(E_SYNTAX, line.lineNum);
}


/* The function takes an instance of a guidance line and inserts its encoding into the output array. */
static void codeGuidance(GuidanceLine line)
{
    output[line.address + ic - START] = line.code;
}


/* The function releases the dynamically assigned tables whose use has ended */
static void freeTables()
{
    free(memory);
    free(data);
}


/* ------------ The main function in the file ------------ */

/* The function makes the second transition on the input:
It encodes the directive sentences and guidance sentences according to the rules
 and returns an array of encoding memory words */
int* secondIteration()
{
    size_t i;
    output = (int*)malloc((dc + ic - START)*sizeof(int));
    externs = (ExternSymbol*)malloc(sizeof(ExternSymbol));
    numExterns = 0;
    for(i = 0; i < numLines; ++i)
    {
        codeDirectiveLine(memory[i]);
    }

    for(i = 0; i < numGuidance; ++i)
        codeGuidance(data[i]);

    freeTables();
    return output;
}


/* The function takes the range of bits it has change in the first memory word of the current directive sentence,
 * and it takes a number that the bits it has to light in the requested bits are lit. */
void setCurrWordBits(int start, int stop, int number)
{
    size_t i;
    number = number <<  start;
    for (i = 0; i < 24; ++i)
        currentWord = currentWord | number;
}