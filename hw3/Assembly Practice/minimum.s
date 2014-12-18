# Nathan Owe
#
# THIS FILE, UNLIKE THE POWER.S FILE, WILL ONLY RUN IN 32 BITS
# BECAUSE IT USES A C METHOD AND IS 32 BITS, YOU MUST RUN LIKE SO:
#
# 	gcc -m32 -o minimum minimum.s
#
# minimum.s 
#
# Find the the maximum number in a list of numbers, written in assembly
#
# %edi will hold the current position in the list
# %ebx will hold the current highest value in the list
# %eax will hold the current element being examined.
#
# The following memory locations are used:
# data_items - contains the item data. A 0 is used to terminate the data.
#
# For each element in the list, we will check for the following:
# If element == -1 : exit
# Increase the current position (%edi)
# Load the next value in the list into the current value register.
# Compare the current value (%eax) with the current high value (%ebx): replace if necessary
# Repeat
#
# We accomplish "if" structures by using jumps in assembly, in this case, either the conditional,
# or unconditional jump.

.section .data

data_items:  #Defining the data data_items
	.quad 3,67,34,222,45,75,54,34,44,33,22,11,66,0

frmtstring:
	.string "Result: %ld\n"

.section .text

.globl main
main:

movq  $0, %rcx			#move 0 into the index register
movq  data_items(,%rcx,8),%rax 	#load the first bye of data
movq  %rax, %rbx			#since this is the first item, %eax is the biggest

cmpq  $0, %rax			#check to see if we've hit the zero,  the end
je loop_exit			#If the previous is true, this line will be executed


start_loop:

incq  %rcx			#load next value(incl or inc means increment by one)
movq  data_items(,%rcx,8), %rax

cmpq  $0, %rax			#check to see if we've hit the zero, which indicates the end
je   loop_exit			#If the previous is true, this line will be executed

cmpq %rbx, %rax			#compare values
jge  start_loop			#jump to loop beginning if the new one is bigger (jge is conditional)

movq  %rax, %rbx			#move the value as the largest
jmp  start_loop			#jump to loop beginning (jmp is unconditional)

loop_exit:
#%ebx is the return value, and it already has the number we need

movq  	%rbx, %rsi
movq  	$frmtstring, %rdi

call 	printf

movq 	$0, %rax
 
ret

#int $0x80

