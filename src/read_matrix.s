.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

	# Prologue

	addi sp, sp, -20
	sw ra, 16(sp)
	sw a0, 12(sp)
	sw a1, 8(sp)
	sw a2, 4(sp)
	sw a3, 0(sp)

	li a1, 0
	jal ra, fopen
	li t0, -1
	beq t0, a0, fopen_error

	addi sp, sp, -4
	sw a0, 0(sp)

	lw a1, 12(sp)
	li a2, 4
	jal ra, fread
	li t1, 4
	bne a0, t1, fread_error

	lw a0, 0(sp)
	lw a1, 8(sp)
	li a2, 4
	jal ra, fread
	li t1, 4
	bne a0, t1, fread_error

	lw t0, 12(sp)
	lw t0, 0(t0)
	lw a0, 8(sp)
	lw a0, 0(a0)
	mul a0, a0, t0
	slli a0, a0, 2
	addi sp, sp, -4
	sw a0, 0(sp)
	jal ra, malloc
	beq a0, zero, malloc_error
	addi sp, sp, -4
	sw a0, 0(sp)

	mv a1, a0
	lw a0, 8(sp)
	lw a2, 4(sp)
	jal ra, fread
	lw t0, 4(sp)
	bne t0, a0, fread_error

	lw a0, 8(sp)
	jal ra, fclose
	bne a0, zero, fclose_error

	lw a0, 0(sp)
	lw ra, 28(sp)
	addi sp, sp, 32
	ret

malloc_error:
	li a0, 26
	j exit

fopen_error:
	li a0, 27
	j exit

fclose_error:
	li a0, 28
	j exit

fread_error:
	li a0, 29
	j exit
