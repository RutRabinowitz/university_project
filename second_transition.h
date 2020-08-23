


#ifndef C_MMN_14_SECOND_TRANSITION_H
#define C_MMN_14_SECOND_TRANSITION_H

#include "first_transition.h"

/*The function gets a string and checks that it represents an integer*/
bool isNumber(const char * str);


/* The function takes the range of bits it has change in the first memory word of the current directive sentence,
 * and it takes a number that the bits it has to light in the requested bits are lit. */
void setCurrWordBits(int start, int stop, int number);


/*The function makes the second transition on the input:
It encodes the directive sentences and guidance sentences according to the rules
 and returns an array of encoding memory words*/
int* secondIteration();

#endif /*C_MMN_14_SECOND_TRANSITION_H*/
