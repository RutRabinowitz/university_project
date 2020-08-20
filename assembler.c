
#include "code_func.h"
#include "read_files.h"
#define LEN 31

/*
The program receives files with the extension as,
outputs files with identical names with obj extension,
In case there are no mistakes. If there are errors in a particular file,
The program will print an error indicating the file name, line,
And the error type and will not produce a suitable file as output.
The program will create obj files only for files where no errors were found.
 */

int main(int argc, char *argv[]) {
    init();
    readFiles(argc, argv);
    return 0;
}