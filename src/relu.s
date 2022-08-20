.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	# Prologue
	addi a1, a1, -1
	blt a1, zero, error

loop:
	blt a1, zero, done
	slli t0, a1, 2
	add t1, a0, t0
	lw t2, 0(t1)
	addi a1, a1, -1
	bge t2, zero, loop
	li t2, 0
	sw t2, 0(t1)
	j loop

done:
	ret

error:
	li a0, 36
	j exit
