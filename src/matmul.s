.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

	# Error checks
	li t0, 1
	blt a1, t0, error
	blt a2, t0, error
	blt a4, t0, error
	blt a5, t0, error
	bne a2, a4, error



	# Prologue
	li t1, 0
	mv t2, a1
	mv t3, a5
	mv t4, a4
	mv t5, a0
	mv t6, a3
	addi sp, sp, -8
	sw ra, 4(sp)
	sw a6, 0(sp)

outer_loop_start:
	li t0, 0

inner_loop_start:
	slli a2, t4, 2
	mul a1, a2, t0
	add a0, a1, t5
	slli a2, t1, 2
	add a1, t6, a2
	mv a2, t4
	li a3, 1
	mv a4, t3

	addi sp, sp, -28
	sw t0, 24(sp)
	sw t1, 20(sp)
	sw t2, 16(sp)
	sw t3, 12(sp)
	sw t4, 8(sp)
	sw t5, 4(sp)
	sw t6, 0(sp)
	jal ra, dot
	lw t0, 24(sp)
	lw t1, 20(sp)
	lw t2, 16(sp)
	lw t3, 12(sp)
	lw t4, 8(sp)
	lw t5, 4(sp)
	lw t6, 0(sp)
	addi sp, sp, 28 

	slli a1, t3, 2
	mul a2, a1, t0
	slli a3, t1, 2
	add a3, a3, a2
	lw a4, 0(sp)
	add a3, a3, a4
	sw a0, 0(a3)

	addi t0, t0, 1
	blt t0, t2, inner_loop_start

inner_loop_end:
	addi t1, t1, 1
	blt t1, t3, outer_loop_start

outer_loop_end:
	lw ra, 4(sp)
	addi sp, sp, 8


	# Epilogue


	ret

error:
	li a0, 38
	j exit