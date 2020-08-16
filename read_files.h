//
// Created by linux on 8/14/20.
//

#ifndef C_MMN_14_READ_FILES_H
#define C_MMN_14_READ_FILES_H
#include <stddef.h>

typedef enum Error{
    E_SYNTAX,
    E_FIRST_OPERAND,
    E_SECOND_OPERAND,
    E_SYMBOL,
    E_FILE,
    //redeclaration of
    E_REDECLARATION,
    E_EXTRA_OPERAND
}Error;

void read_files(int argc, char *argv[]);
void error(Error err, size_t numLine);

#endif //C_MMN_14_READ_FILES_H
