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
    E_SYMBOL
}Error;


void error(Error err, size_t numLine);

#endif //C_MMN_14_READ_FILES_H
