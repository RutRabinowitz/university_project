; err_prog
num: .data 4 invalid syntax
ads r3, hi
dec W
clr r3, r2
add r1,#3
sub num, #4
cmp &num, &f
END: stop 3
jmp &ext
.extern ext


