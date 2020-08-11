//
// Created by linux on 8/10/20.
//

#ifndef C_MMN_14_FIRST_TRANSITION_H
#define C_MMN_14_FIRST_TRANSITION_H

#include <stdio.h>
#define LEN 31

void first_iteration(const char * fileName);
typedef enum bool{false = 0, true = 1} bool;

typedef struct Symbol
{
    char symbolName[LEN];
    size_t address;
    bool isInstruction;
    bool isExtern;
}Symbol;

typedef struct DirectiveLine
{
    char text[80];
    size_t address;
}DirectiveLine;

typedef struct GuidanceLine
{
    int code;
    size_t address;
}GuidanceLine;


#endif //C_MMN_14_FIRST_TRANSITION_H
