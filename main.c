#include <stdio.h>
#include "code_func.h"
#include <string.h>
#include "first_transition.h"
#include "second_transition.h"
#include "read_files.h"
#define LEN 31






int main(int argc, char *argv[]) {
    init();
    read_files(argc, argv);
    return 0;
}