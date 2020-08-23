; err_prog

;invalid syntax
num: .data 4 invalid syntax

;invalid syntax 
ads r3, hi

;udefined symbol
dec W

;too many operands
clr r3, r2

;Illegal addressing method for the second operand
add r1,#3

;Illegal addressing method for the second operand
sub num, #4

;Illegal addressing method for the first operand
;Illegal addressing method for the second operand
cmp &num, &f

;too many operands
END: stop 3

;Illegal addressing method for the second operand
jmp &ext

.extern ext


