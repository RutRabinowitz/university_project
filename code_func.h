//
// Created by linux on 7/20/20.
//

#ifndef C_MMN_14_CODE_FUNC_H
#define C_MMN_14_CODE_FUNC_H

const char * instructions_names[15];
int instructions_opcodes[15][2];
int getOpcod(const char *instruction_name);
int getOpcode(const char *instruction_name);
typedef struct Directive{
    int opcode;
    int funct;
    char * name;
    int AddressingMethodSrc[4];
    int AddressingMethodDst[4];
}Directive;
void init();
Directive directives[16];
#endif //C_MMN_14_CODE_FUNC_H
