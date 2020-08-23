#ifndef C_MMN_14_DST_OPERAND_H
#define C_MMN_14_DST_OPERAND_H
#include <stddef.h>
#include "first_transition.h"


/* The function takes a directive sentence that has a destination operand,
 checks what the destination operand is and what its addressing method is and sets the appropriate bits.*/
Word dstOperand(DirectiveLine line, size_t j, size_t directiveIdx, Word result);

#endif /*C_MMN_14_DST_OPERAND_H*/
