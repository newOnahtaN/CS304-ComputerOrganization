# 32-bit x86 GAS Linux assembly

.section .data
fmtstr:
	.string "%s\n"
teststr:
	.string "Hello, world!"
fmtdec:
	.string "%d\n"

.section .text
.globl main
main:

push $teststr
push $fmtstr


call printf
add $8, %esp

push $5
push $fmtdec

call printf
add $8, %esp

ret
