# Nathan Owen
# Maximum.s 
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
# If element == 0 : exit
# Increase the current position (%edi)
# Load the next value in the list into the current value register.
# Compare the current value (%eax) with the current high value (%ebx): replace if necessary
# Repeat
#
# We accomplish "if" structures by using jumps in assembly, in this case, either the conditional,
# or unconditional jump.

.section .data

data_items:  #Defining the data data_items
.long 3,67,34,222,45,75,54,34,44,33,22,11,66,0

.section .text

.globl _start
_start:

movl $0, %edi				#move 0 into the index register
movl data_items(,%edi,4),%eax #load the first bye of data
movl %eax, %ebx				#since this is the first item, %eax is the biggest


start_loop:

cmpl $0, %eax				#check to see if we've hit the examined
je loop_exit				#If the previous is true, this line will be executed

incl %edi					#load next value    (incl or inc means increment by one)
movl data_items(,%edi,4), %eax

cmpl %ebx, %eax				#compare values
jle start_loop				#jump to loop beginning if the new one isn't bigger (jle is unconditional)

movl %eax, %ebx				#move the value as the largest
jmp start_loop				#jump to loop beginning

loop_exit:
#%ebx is the return value, and it already has the number we need
movl $1, %eax				#1 is the exit() syscall
int $0x80
