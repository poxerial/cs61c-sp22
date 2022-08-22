.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

	# Prologue
	addi sp, sp, -24
	sw ra, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)

	li a1, 1
	jal ra, fopen
	li t0, -1
	beq t0, a0, fopen_error
	sw a0, 16(sp) 

	addi a1, sp, 8
	li a2, 1
	li a3, 4
	jal ra, fwrite
	li t0, 1
	bne t0, a0, fwrite_error

	lw a0, 16(sp)
	addi a1, sp, 12
	li a2, 1
	li a3, 4
	jal ra, fwrite
	li t0, 1
	bne t0, a0, fwrite_error

	lw a0, 16(sp)
	lw a1, 4(sp)
	lw a2, 12(sp)
	lw a3, 8(sp)
	mul a2, a2, a3
	sw a2, 20(sp)
	li a3, 4
	jal ra, fwrite
	lw t0, 20(sp)
	bne t0, a0, fwrite_error

	lw a0, 16(sp)
	jal ra, fclose
	bne zero, a0, fclose_error

	# Epilogue
	lw ra, 0(sp)
	addi sp, sp, 24

	ret

fwrite_error:
	li a0, 30
	j exit

fopen_error:
	li a0, 27
	j exit

fclose_error:
	li a0, 28
	j exit


