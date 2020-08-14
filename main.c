#include <stdio.h>
#include "instruction_validation.h"
#include "code_func.h"
#include <string.h>
#include "first_transition.h"
#include "second_transition.h"
#define LEN 31






int main(int argc, char *argv[]) {
    init();
    for (int i = 0; i < argc; i++)
    {
        int len = strlen(argv[i]);
        const char *last_three = &argv[i][len - 3];
        if (!strcmp(last_three, ".as"))
        {
            char *fileName = str_slice(argv[i], 0, len - 2);
            fileName = strcat(fileName, "txt");
            first_iteration(fileName);
            second_iteration(fileName);
        }
    }
    return 0;
}