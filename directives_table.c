#include "directives_table.h"
#include <string.h>
#include <stdlib.h>

#define NUM_DIR 16


/* -------------General auxiliary function in the program-------------- */
/* Extracts a selection of string and return a new string or NULL.
 * It supports both negative and positive indexes.
 */
char * str_slice(const char str[], int slice_from, int slice_to)
{
    char *buffer;
    size_t str_len, buffer_len;

    if (str[0] == '\0')
        return NULL;

    if (slice_to < 0 && slice_from < slice_to) {
        str_len = strlen(str);

        if (abs(slice_to) > str_len - 1)
            return NULL;

        if (abs(slice_from) > str_len)
            slice_from = (-1) * str_len;

        buffer_len = slice_to - slice_from;
        str += (str_len + slice_from);

    } else if (slice_from >= 0 && slice_to > slice_from) {
        str_len = strlen(str);

        if (slice_from > str_len - 1)
            return NULL;

        buffer_len = slice_to - slice_from;
        str += slice_from;

    } else
        return NULL;

    buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}


/* ---------- Global tables for directives information ---------- */

char * directivesNames[NUM_DIR] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                                       "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};


int directivesOpcodes[NUM_DIR][2] =
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


int addressingMethodSrc[NUM_DIR][ADDRESS_MTD_NUM] = {
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


int addressingMethodDst[NUM_DIR][ADDRESS_MTD_NUM] = {
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


/* ----------- The main functions in this file ----------- */

/* The function initializes the tables of directives - for each directive its name,
   opcode and addressing methods are allowed for the source operand and the target operand.*/
void init()
{
    size_t i;
    size_t j;
    for(i = 0; i < NUM_DIR; i++)
    {
        directives[i].opcode = directivesOpcodes[i][0];
        directives[i].funct = directivesOpcodes[i][1];
        directives[i].name = directivesNames[i];
        for (j = 0; j < ADDRESS_MTD_NUM; ++j)
        {
            directives[i].AddressingMethodSrc[j] = addressingMethodSrc[i][j];
            directives[i].AddressingMethodDst[j] = addressingMethodDst[i][j];
        }
    }
}


/* The function takes a string and checks if it is the name of a directive.
   If so, it returns the directive index in the directive table. Otherwise, it returns -1. */
int getDirIdx(const char *directive_name)
{
    int i;
    for(i = 0; i < NUM_DIR; ++i)
    {
        if(!strcmp(directive_name, directivesNames[i]))
        {
            return i;
        }
    }
    return -1;
}