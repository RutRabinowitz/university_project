


#ifndef C_MMN_14_SECOND_TRANSITION_H
#define C_MMN_14_SECOND_TRANSITION_H

#include "first_transition.h"

/*The function gets a string and checks that it represents an integer*/
bool isNumber(const char * str);

/*The function changes the bits of the first memory word of the current instruction.
 It gets the range of bits to change, and the content of the change. */
void setCurrWordBits(int start, int stop, int number);

/*The function makes the second transition on the input:
It encodes the directive sentences and guidance sentences according to the rules
 and returns an array of encoding memory words*/
int* secondIteration();

#endif /*C_MMN_14_SECOND_TRANSITION_H*/
