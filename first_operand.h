//
// Created by linux on 8/12/20.
//

#ifndef C_MMN_14_FIRST_OPERAND_H
#define C_MMN_14_FIRST_OPERAND_H
#include <stddef.h>
#include "first_transition.h"



typedef struct Word{
    bool isFirst;
    int firstWord;
    bool isSecond;
    int secondWord;
} Word;

Word first_operand(DirectiveLine line, size_t j, size_t directiveIdx);

#endif //C_MMN_14_FIRST_OPERAND_H
