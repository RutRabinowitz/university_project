#ifndef C_MMN_14_SRC_OPERAND_H
#define C_MMN_14_SRC_OPERAND_H
#include <stddef.h>
#include "first_transition.h"


/* The function gets a directive sentence that has a source operand,
 checks what the source operand is and what its addressing method is and sets the appropriate bits.*/
Word srcOperand(DirectiveLine line, size_t j, size_t directiveIdx);

#endif /*C_MMN_14_SRC_OPERAND_H*/
