//
// Created by linux on 7/20/20.
//

#ifndef C_MMN_14_CODE_FUNC_H
#define C_MMN_14_CODE_FUNC_H


int getOpcode(const char *instruction_name);
typedef struct Directive{
    int opcode;
    int funct;
    char * name;
    int AddressingMethodSrc[4];
    int AddressingMethodDst[4];
}Directive;

char * str_slice(const char str[], int slice_from, int slice_to);
void init();
Directive directives[16];
#endif //C_MMN_14_CODE_FUNC_H
