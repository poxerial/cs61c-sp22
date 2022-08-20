.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
	addi a1, a1, -1
	blt a1, zero, error

	li t0, 0x80000000 # t0 = int_min
	addi t1, a1, 0

loop:
	slli t2, a1, 2
	add t3, a0, t2
	lw t4, 0(t3)
	blt t0, t4, is_larger
loop_continue:
	addi a1, a1, -1
	bge a1, zero, loop
	add a0, zero, t1
	ret

is_larger:
	add t1, zero, a1
	add t0, zero, t4
	j loop_continue

error:
	li a0, 36
	j exit

