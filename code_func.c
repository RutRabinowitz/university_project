//
// Created by linux on 7/20/20.
//

#include "code_func.h"
#include <string.h>
const char * instructions_names[15] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                                       "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};

int instructions_opcodes[15][2] =
        {
        {0, 0},
        {1, 0},
        {2, 1},
        {2, 2},
        {4, 0},
        {5, 1},
        {5, 2},
        {5, 3},
        {5, 4},
        {9, 1},
        {9, 2},
        {9, 3},
        {12, 0},
        {13, 0},
        {14, 0},
        {15, 0}
};

int addressingMethodSrc[15][4] = {
        {1, 1, 0, 1},
        {1, 1, 0, 1},
        {1, 1, 0, 1},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
};

int addressingMethodDst[15][4] = {
        {0, 1, 0, 1},
        {1, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 1, 0, 1},
        {1, 1, 0, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
};

void init()
{
    size_t i;
    size_t j;
    for(i = 0; i < 15; i++)
    {
        directives[i].opcode = instructions_opcodes[i][0];
        directives[i].funct = instructions_opcodes[i][1];
        directives[i].name = instructions_names[i];
        for (j = 0; j < 4; ++j)
        {
            directives[i].AddressingMethodSrc[j] = addressingMethodSrc[i][j];
            directives[i].AddressingMethodDst[j] = addressingMethodDst[i][j];
        }
    }
}

int getOpcod(const char *instruction_name)
{
    int i;
    for(i = 0; i < 15; ++i)
    {
        if(!strcmp(instruction_name, instructions_names[i]))
        {
            return i;
        }
    }
    return -1;
}


int getOpcode(const char *instruction_name)
{
    int i;
    for(i = 0; i < 15; ++i)
    {
        if(!strcmp(instruction_name, instructions_names[i]))
        {
            return 1;
//            return instructions_opcodes[i];
        }
    }
    return 0;
//    return NULL;
}