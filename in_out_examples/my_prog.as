; my_prog.as

B: .data 900
inc r1
jmp &LINE
HELLO: sub #3, r1
cmp A, B
LINE: sub r5, r6
A: .data 2
STR: .string "Hello world"
add #-567, u
.extern u
.entry B

