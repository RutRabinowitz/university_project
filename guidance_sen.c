
#include <stdlib.h>
#include <string.h>
#include "directives_table.h"
#include "first_transition.h"
#include "read_write_files.h"

#define SPC 32
#define TAB 9
#define NEW_LINE 10


/*----------------Defining global variables of the system----------------*/

extern Symbol *symbolTable;
extern DirectiveLine *memory;
extern GuidanceLine *data;
extern ExternSymbol *entrySymbols;

extern size_t ic;
extern size_t dc;
extern size_t symbolNum;
extern size_t numEntries;
extern size_t numLines;
extern size_t numGuidance;


/* ------------------- Auxiliary functions ------------------- */

/* The function gets a string and checks if it represents an integer (returns a Boolean value).*/
bool isNumber(const char * str)
{
    int i, len = strlen(str);
    bool flag = true;
    
    if (str && str[0] == '0')
        flag = false;

    for(i = 0; i < len && flag; i++)
    {
        if(!((str[i] >= '0' && str[i] <= '9')  || (i == 0 && (str[i] == '-' || str[i] == '+'))))
            flag = false;
    }
    return flag;
}


/*The function takes a line and an index and checks if from the index onwards
  there is a character that is not a new tab / space / line.
  This is to make sure that there are no words / characters later in the line illegally.*/
static void checkSyntaxErrors(const char * line, size_t i, size_t lineNum)
{
    while(line[i] && (line[i] == NEW_LINE || line[i] == TAB ||line[i] == SPC))
    {
        i++;
    }
    if(line[i] && !(line[i] == NEW_LINE || line[i] == TAB ||line[i] == SPC))
        error(E_SYNTAX, lineNum);
}


/*The function gets a data to insert into the data table. The function enters the table.*/
static void insertData(int c)
{
    numGuidance++;
    data = (GuidanceLine *) realloc(data, numGuidance * sizeof(GuidanceLine));
    data[numGuidance - 1].code = c;
    data[numGuidance - 1].address = dc;
    dc++;
}

/*The function takes a string that is either an '.data' guidance sentence or invalid.
  It checks whether the sentence is indeed an 'data type guidance.
  If so - it inserts it into the data table and updates the dc. If not, it makes an error.*/
static void dataGuidance(const char * line, size_t i, size_t lineNum)
{
    size_t j = i + 5;
    size_t k;
    if(strlen(line) < i + 7 || strcmp("data ", str_slice(line, i + 1, i + 6)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }
    while (line[j])
    {
        
        while (line[j] && (line[j] == NEW_LINE || line[j] == SPC || line[j] == TAB)){j++;}
        k = j;
        while (line[j] && line[j++] != ','){}

        if ((line[k - 1] == SPC || line[k - 1] == NEW_LINE || line[k - 1] == TAB) && (j - 1 > k && isNumber(str_slice(line, k, j - 1))))
        {
            insertData(atoi(str_slice(line, k, j - 1)));
        }
        else
        {
            error(E_SYNTAX, lineNum);
            return;
        }
    }
    checkSyntaxErrors(line, j, lineNum);
}


/*The function takes a string that is either an '.string' guidance sentence or invalid.
  It checks whether the sentence is indeed an '.string' type guidance.
  If so - it inserts into the data table and updates the dc. If not, it makes an error.*/
static void stringGuidance(const char * line, size_t i, size_t lineNum)
{
    size_t j = i + 11;

    if(strlen(line) < i + NEW_LINE || strcmp("string \"", str_slice(line, i + 1, i + TAB)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }

    while(line[j] && line[j] != -30)
    {
        insertData(line[j]);
        j++;
    }
    if(line[j - 2] == -30)
        error(E_SYNTAX, lineNum);
    insertData('\0');
    /*make sure that there are no words / characters later in the line illegally.*/
    checkSyntaxErrors(line, j - 1, lineNum);
}


/*The function takes a string that is either an '.entry' guidance sentence or invalid.
  It checks whether the sentence is indeed an entry type guidance.
  If so - it inserts it into the entries table. If not, it makes an error.*/
static void entryGuidance(const char * line, size_t i, size_t lineNum)
{
    size_t j;

    if(strlen(line) < i + TAB || strcmp("entry ", str_slice(line, i + 1, i + 7)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }
    i += 7;
    while (line[i] && (line[i] == NEW_LINE || line[i] == SPC || line[i] == TAB)){i++;}
    j = i;
    while(line[j] && !(line[j] == NEW_LINE || line[j] == SPC || line[j] == TAB)){j++;}

    if (i < j)
    {
        numEntries++;
        entrySymbols = (ExternSymbol*) realloc(entrySymbols, numEntries * sizeof(ExternSymbol));
        strcpy(entrySymbols[numEntries - 1].name, str_slice(line, i, j));
    }
    /*make sure that there are no words / characters later in the line illegally.*/
    checkSyntaxErrors(line, j, lineNum);
}


/*The function takes a string that is either an external guidance sentence or invalid.
  It checks whether the sentence is indeed an external type guidance.
  If so - it inserts it into the symbol table. If not, it makes an error.*/
static void externalGuidance(const char * line, size_t i, size_t lineNum)
{
    size_t j;
    if(strlen(line) < i + NEW_LINE || strcmp("extern ", str_slice(line, i + 1, i + 8)))
    {
        error(E_SYNTAX, lineNum);
        return;
    }

    i += 8;
    while (line[i] && (line[i] == NEW_LINE || line[i] == SPC || line[i] == TAB)){i++;}
    j = i;
    while(line[j] && !(line[j] == NEW_LINE || line[j] == SPC || line[j] == TAB)){j++;}

    if (j > i)
    {
        symbolNum++;
        symbolTable = (Symbol *) realloc(symbolTable, symbolNum * sizeof(Symbol));
        strcpy(symbolTable[symbolNum - 1].symbolName, str_slice(line, i, j));
        symbolTable[symbolNum - 1].address = 0;
        symbolTable[symbolNum - 1].isExtern = 1;
        symbolTable[symbolNum - 1].isDirective = false;
    }
    else
    {
        error(E_SYNTAX, lineNum);
        return;
    }
    /*make sure that there are no words / characters later in the line illegally.*/
    checkSyntaxErrors(line, j, lineNum);
}



/* ------------ The main function in the file ------------ */

/*The function takes a line that is either a guidance sentence,
 or that is invalid. The function checks if it is valid and updates the dc and the data accordingly. */
void guidanceSen(const char * line, size_t i, size_t lineNum)
{
    if (i + 2 < strlen(line))
    {
        char* s = str_slice(line, i + 1, i + 3);

        if (!strcmp(s, "da"))
        {
            dataGuidance(line, i, lineNum);
        }
        else if (!strcmp(s, "st"))
        {
            stringGuidance(line, i, lineNum);
        }

        else if (!strcmp(s, "en")){
            entryGuidance(line, i, lineNum);
        }

        else if (!strcmp(s, "ex"))
        {
            externalGuidance(line, i, lineNum);
        }
        else
            error(E_SYNTAX, lineNum);
    }
}

