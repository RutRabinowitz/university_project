#ifndef C_MMN_14_READ_FILES_H
#define C_MMN_14_READ_FILES_H
#include <stddef.h>

typedef enum Error
{
    E_SYNTAX,
    E_FIRST_OPERAND,
    E_SECOND_OPERAND,
    E_SYMBOL,
    E_FILE,
    E_REDECLARATION,
    E_EXTRA_OPERAND
}Error;

/*The function gets the file names and number of files.
It goes through each file twice and if no errors are found in it, it creates the appropriate files.*/
void readFiles(int argc, char *argv[]);

/* The function gets the type of error and the line in which it was found, and prints an appropriate error */
void error(Error err, size_t numLine);

#endif /*C_MMN_14_READ_FILES_H*/
