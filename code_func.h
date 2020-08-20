
#ifndef C_MMN_14_CODE_FUNC_H
#define C_MMN_14_CODE_FUNC_H

#define ADDRESS_MTD_NUM 4
#define START 100

/*The function gets a string of the name of a directive.
 If this directive exists, it returns its opcode. Otherwise it returns -1.*/
int getOpcode(const char *instruction_name);

typedef struct Directive{
    int opcode;
    int funct;
    char * name;
    int AddressingMethodSrc[4];
    int AddressingMethodDst[4];
}Directive;


/*The function gets a string, and a range of indexes by which it cuts the string -
  from the start index to the end index.*/
char * str_slice(const char str[], int slice_from, int slice_to);


/* The function initializes the tables of directives - for each directive its name,
   opcode and addressing methods are allowed for the source operand and the target operand.*/
void init();

Directive directives[16];
#endif /*C_MMN_14_CODE_FUNC_H*/
