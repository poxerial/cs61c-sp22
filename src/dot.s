.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

	# Prologue
	li t0, 1
	blt a2, t0, error_36
	blt a3, t0, error_37
	blt a4, t0, error_37

	li t0, 0
	li t3, 0
	li t4, 0
	slli a3, a3, 2
	slli a4, a4, 2

loop_start:
	add t1, a0, t3
	add t2, a1, t4
	lw t5, 0(t1)
	lw t6, 0(t2)
	mul t1, t5, t6
	add t0, t0, t1

	add t3, t3, a3
	add t4, t4, a4
	addi a2, a2, -1
	blt zero, a2, loop_start

loop_end:
	add a0, zero, t0


	# Epilogue


	ret

error_36:
	li a0, 36
	j exit

error_37:
	li a0, 37
	j exit