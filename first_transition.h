
#ifndef C_MMN_14_FIRST_TRANSITION_H
#define C_MMN_14_FIRST_TRANSITION_H

#include <stdio.h>

#define LEN 31
#define LINE_LEN 80

/* The function makes the first transition over the file:
 it reads line by line in the file, checks the type of sentence,
 saves the directives and guidance sentences separately and also builds a table of symbols. */
void firstIteration(FILE *fp);

typedef enum bool{false = 0, true = 1} bool;


/* Data structure for storing symbol information - for each symbol its name,
  address, if it is in a directive sentence and its type (extern or not)*/
typedef struct Symbol
{
    char symbolName[LEN];
    size_t address;
    bool isDirective;
    /* isExtern is 1 if the symbol is extern, and 2 otherwise. */
    int isExtern;
}Symbol;

/* Data structure for storing external symbol information - for each symbol its name,
   and its number of line. */
typedef struct ExternSymbol {
    char name[LINE_LEN];
    size_t numLine;
}ExternSymbol;


/* Data structure for storing directive line information - for each directive line its text,
   address and its number of line. */
typedef struct DirectiveLine
{
    char text[LINE_LEN];
    size_t address;
    size_t lineNum;
}DirectiveLine;

/* Data structure for storing guidance line information - for each guidance line its code,
   and its address. */
typedef struct GuidanceLine
{
    int code;
    size_t address;
}GuidanceLine;


/* Data structure for storing the extra memory word in each directive line - for each directive line
   if there are second and third memory word, the words, and if the words are addresses of external symbols. */
typedef struct Word{
    bool isSecond;
    int secondWord;
    bool isThird;
    int thirdWord;
    int extSymbols[2];
} Word;


/*The function gets a symbol name - string, and returns its
 index if the symbol is in the table and -1 otherwise */
int isInTable(const char * symbol);

#endif /*C_MMN_14_FIRST_TRANSITION_H*/
