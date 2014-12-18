#PURPOSE: Program to illustrate how functions work
# This program will compute the value of
# 2^3 + 5^2
#
#Everything in the main program is stored in registers,
#so the data section doesn’t have anything.
.section .data
fmtstr:
	.string "%s"
fmtdec:
	.string "%d\n"
resultstr:
	.string "Result: "

.section .text
.globl main
main:

push 	$3		#push second argument
push 	$2		#push first argument
call 	power 		#call the function
add 	$16,%rsp 	#move the stack pointer back

push 	%rax 		#save the first answer before
			#calling the next function
push	$2		#push second argument
push	$5 		#push first argument
call 	power 		#call the function
add 	$16, %rsp 	#move the stack pointer back
pop 	%rbx 		#The second answer is already
			#in %rax. We saved the
			#first answer onto the stack,
			#so now we can just pop it
			#out into %rbx
add 	%rax, %rbx 	#add them together
			#the result is in %rbx

movq	$resultstr, %rsi#Set the %rsi register to the result string
movq	$fmtstr, %rdi	#Set the %rdi register to the format string
movq	$0,%rax		#Set the return value, rax, to zero

			#The above operations are necessary for x64 bit
			#assembly in order to call functions. Ints must
			#be passed into certain registers instead of being
			#pushed onto the stack.
call	printf

movq	%rbx, %rsi	#Put the result of the program onto %rsi
movq	$fmtdec, %rdi
movq	$0, %rax

call	printf

ret
#mov 	$1, %rax 	#exit (%rbx is returned)
#int 	$0x80



#PURPOSE: This function is used to compute
# the value of a number raised to
# a power.
#
#INPUT: First argument - the base number
# Second argument - the power to
# raise it to
#
#OUTPUT: Will give the result as a return value
#
#NOTES: The power must be 1 or greater
#
#VARIABLES:
# %rbx - holds the base number
# %rcx - holds the power
#
# -4(%rbp) - holds the current result
#
# %eax is used for temporary storage
#

.type power, @function
power:

push 	%rbp 			#save old base pointer
mov 	%rsp, %rbp 		#make stack pointer the base pointer
sub 	$8, %rsp 		#get room for our local storage

mov 	16(%rbp), %rbx 		#put first argument in %rbx
mov 	24(%rbp), %rcx		#put second argument in %rcx
mov 	%rbx, -8(%rbp) 		#store current result

power_loop_start:
cmp 	$1, %rcx 		#if the power is 1, we are done
je 	end_power

mov 	-8(%rbp), %rax 		#move the current result into %eax
imul 	%rbx, %rax 		#multiply the current result by
				#the base number
mov 	%rax, -8(%rbp) 		#store the current result
dec 	%rcx 			#decrease the power
jmp 	power_loop_start	#run for the next power

end_power:
mov 	-8(%rbp), %rax 		#return value goes in %rax
mov 	%rbp, %rsp 		#restore the stack pointer
pop 	%rbp			#restore the base pointer
ret
