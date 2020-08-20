
#ifndef C_MMN_14_FIRST_TRANSITION_H
#define C_MMN_14_FIRST_TRANSITION_H

#include <stdio.h>

#define LEN 31
#define LINE_LEN 80

/* The function makes the first transition over the file:
 it reads line by line in the file, checks the type of sentence,
 saves the directives and guidance sentences separately and also builds a table of symbols. */
void firstIteration(const char * fileName);

typedef enum bool{false = 0, true = 1} bool;

typedef struct Symbol
{
    char symbolName[LEN];
    size_t address;
    bool isInstruction;
    int type;
}Symbol;


typedef struct EntrySymbol {
    char name[LINE_LEN];
    size_t numLine;
}EntrySymbol;

typedef struct DirectiveLine
{
    char text[LINE_LEN];
    size_t address;
    size_t lineNum;
}DirectiveLine;

typedef struct GuidanceLine
{
    int code;
    size_t address;
}GuidanceLine;

typedef struct Word{
    bool isFirst;
    int firstWord;
    bool isSecond;
    int secondWord;
    int extSymbols[2];
} Word;


/*The function gets a symbol name - string, and returns its
 index if the symbol is in the table and -1 otherwise */
int isInTable(const char * symbol);

#endif /*C_MMN_14_FIRST_TRANSITION_H*/
