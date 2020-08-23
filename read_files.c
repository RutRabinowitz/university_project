
#include "read_write_files.h"
#include "first_transition.h"
#include "second_transition.h"
#include "directives_table.h"
#include <string.h>
#include <stdlib.h>


bool isValidFile;
extern size_t ic;
extern size_t dc;
char *fileName;
extern size_t numEntries;

extern ExternSymbol *entrySymbols;
extern Symbol *symbolTable;

extern ExternSymbol* externs;
extern size_t numExterns;


/* ----------- Auxiliary functions ----------- */

/* The function takes an error type and line number, and prints an appropriate message. */
static void printError(Error err, size_t numLine)
{
    char * errorMsg[7] = {"Invalid syntax", "Illegal addressing method for the first operand",
                          "Illegal addressing method for the second operand",
                          "No such symbol", "file does not exist", "Redeclaration of symbol",
                          "Too many operands"};
    if (err == 4)
        printf("%s: %s\n", fileName, errorMsg[err]);
    else
        printf("file %s: %ld: error: %s\n", fileName, numLine, errorMsg[err]);
}

/* The function gets a pointer to file and a number and prints appropriately. */
static void printLineNum(int num, FILE *fp)
{
    fprintf(fp,"%07d\t", num);
}

/* Function for development purposes only!
   The function takes an integer and a number of bits,
   and prints the number in binary representation as the number of bits.
static void printBinaryRepresentation(int number, size_t j)
{
    unsigned int musk = 1 << (j - 1);
    int i;

    for (i = 0; i < j; ++i, musk = musk >> 1)
    {
        musk & number? printf("%d", 1):printf("%d", 0);
    }
}*/


/* The function takes an integer and pointer to a file,
   and prints the number in the hexadecimal representation on a 24-bit basis. */
static void printHexNum(int word, FILE *fp)
{
    int musk = 15 << 20;
    size_t i;
    for(i = 0; i < 6; i++, musk = musk >> 4)
    {
        fprintf(fp,"%x", (word&musk)>>(20 - 4*i));
    }
    fprintf(fp,"\n");
}


/* The function takes a string of an extension of a file
 * and creates a file named as the input file with the extension it gets.
 * It returns a pointer to the file. */
static FILE *openFile(const char * extension)
{
    FILE *fp;
    char *newFile = (char*)malloc(sizeof(char)*(strlen(fileName) + 1));
    strcpy(newFile, fileName);
    fp = fopen(strcat(newFile, extension), "w");
    free(newFile);
    return fp;
}


/* The function takes the output, in an array of integers.
 * It prints the output to the ob file. */
static void printToFile(int * output)
{
    FILE *fp = openFile(".ob");
    size_t i;
    fprintf(fp, "%7ld\t%ld\n", ic - START, dc);

    for(i = 0; i < (ic + dc - START); i++)
    {
        printLineNum(i + START, fp);
        printHexNum(output[i], fp);
    }
}


/*The function creates a file with an ent extension and prints the '.entry' symbols and the number of the line. */
static void printEntryFile()
{
    FILE *fp = openFile(".ent");
    size_t i;
    for(i = 0; i < numEntries; i++)
    {
        fprintf(fp,"%s\t%07ld\n", entrySymbols[i].name, symbolTable[isInTable(entrySymbols[i].name)].address);
    }
}


/* The function creates a file with an ext extension
 * and prints the '.extern' symbols and the numbers of the line those symbols are used. */
static void printExtFile()
{
    size_t i;
    FILE *fp = openFile(".ext");

    for(i = 0; i < numExterns; i++)
    {
        fprintf(fp, "%s\t%07ld\n", externs[i].name, externs[i].numLine + START);
    }
}


/* The function releases the dynamically assigned tables whose use has ended */
static void freeTable()
{
    free(externs);
    free(entrySymbols);
    free(symbolTable);
    free(fileName);
    numExterns = 0;
}


/* The function creates all the required output files and prints the output. */
static void printFilesIfValid(int *output)
{
    if(isValidFile)
    {
        printToFile(output);
        if (numEntries)
            printEntryFile();
        if(numExterns)
            printExtFile();
    }
}


/* ----------- The main functions in the file ----------- */

/*The function gets the file names and number of files.
It goes through each file twice and if no errors are found in it, it creates the appropriate files.*/
void readFiles(int argc, char *argv[])
{
    size_t i;
    int * output;
    FILE *fp;
    for (i = 1; i < argc; i++)
    {
        char* file = (char*)malloc(sizeof(char)*strlen(argv[i] + 1));
        fileName = (char*)malloc(sizeof(char)*strlen(argv[i] + 1));
        strcpy(file, argv[i]);
        strcpy(fileName, argv[i]);
        strcat(file, ".as");
        isValidFile = true;
        fp = fopen(file, "r");

        if (fp == NULL)
        {
            error(E_FILE, 0);
            continue;
        }
        firstIteration(fp);
        output = secondIteration();

        if(isValidFile)
            printFilesIfValid(output);

        free(output);
        free(file);
        freeTable();
    }
}


/* The function gets the type of error and the line in which it was found, and prints an appropriate error */
void error(Error err, size_t numLine)
{
    isValidFile = false;
    printError(err, numLine);
}