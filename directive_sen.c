#include "directive_sen.h"
#include <stdlib.h>
#include <string.h>
#include "first_transition.h"
#include "directives_table.h"
#include "read_write_files.h"


#define SPC 32
#define TAB 9
#define NEW_LINE 10

extern DirectiveLine *memory;
extern size_t numLines;
extern size_t ic;

/* ------------------ Auxiliary Functions ---------------- */

/* The function takes a string and an index, and checks if this index and the one after it have a register. */
static bool isNotRegister(const char * line, size_t j)
{
    return (line[j] && !(line[j] == 'r' && j + 1 < strlen(line) && (line[j + 1] >= '0' && line[j + 1] <= '7')));
}


/*The function takes a string of a directive line and a line number and inserts it into the memory. */
static void insertToMemory(const char * line, size_t lineNum)
{
    if (strlen(line))
    {
        numLines++;
        memory = (DirectiveLine*)realloc(memory, numLines * sizeof(DirectiveLine));
        strcpy(memory[numLines - 1].text, line);
        memory[numLines - 1].address = ic;
        memory[numLines - 1].lineNum = lineNum;
    }

}


/* ------------------ The main function in the file ---------------- */

/* The function takes a line that is either a directive,
 or that is invalid. The function checks if it is valid and updates the ic and memory accordingly */
void directiveSen(const char * line, size_t i, size_t lineNum)
{
    size_t j = i + 3;
    insertToMemory(line, lineNum);
    ic++;
    if(!strcmp("stop", str_slice(line, i, i + 4)))
        return;

    while(line[j] && (line[j] == ' ' || line[j] == '\t'))
        j++;
    if (isNotRegister(line, j))
        ic++;
    while(line[j] && !(line[j++] == ',')){}
    j--;

    if (line[j] && line[j] == ',')
    {
        j++;
        while(line[j] && (line[j] == ' ' || line[j] == '\t')) {j++;}

        if (isNotRegister(line, j))
        {
            ic++;
            while(line[j] && (line[j] != NEW_LINE && line[j] != TAB && line[j++] != SPC)){}
            j++;
        }
        else
            j += 3;
    }

    if(line[j] && (line[j] != NEW_LINE && line[j] != TAB && line[j] != SPC))
    {
        error(E_SYNTAX, lineNum);
    }
}
