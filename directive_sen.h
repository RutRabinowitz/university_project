#ifndef C_MMN_14_DIRECTIVE_SEN_H
#define C_MMN_14_DIRECTIVE_SEN_H
#include <stddef.h>

/*The function takes a line that is either a directive sentence,
 or that is invalid. The function checks if it is valid and updates the ic and the memory accordingly */
void directiveSen(const char * line, size_t i, size_t lineNum);

#endif /*C_MMN_14_DIRECTIVE_SEN_H*/
