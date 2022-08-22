.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
	li t0, 5
	bne a0, t0, error_code_31
	addi sp, sp, -68
	sw ra, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)

	lw a0, 4(sp)
	lw a0, 4(a0)
	addi a1, sp, 12
	addi a2, sp, 16
	jal ra, read_matrix #malloc
	sw a0, 20(sp)

	lw t0, 4(sp)
	lw a0, 8(t0)
	addi a1, sp, 24
	addi a2, sp, 28
	jal ra, read_matrix #malloc
	sw a0, 32(sp)

	lw t0, 4(sp)
	lw a0, 12(t0)
	addi a1, sp, 36
	addi a2, sp, 40
	jal ra, read_matrix #malloc
	sw a0, 44(sp)

	lw t0, 12(sp)
	lw t1, 40(sp)
	mul a0, t0, t1
	sw a0, 48(sp)
	slli a0, a0, 2
	jal ra, malloc	
	beq a0, zero, malloc_error
	sw a0, 52(sp)

	mv a6, a0
	lw a0, 20(sp)
	lw a1, 12(sp)
	lw a2, 16(sp)
	lw a3, 44(sp)
	lw a4, 36(sp)
	lw a5, 40(sp)
	jal ra, matmul

	lw a0, 52(sp)
	lw a1, 48(sp)
	jal ra, relu

	lw t0, 24(sp)
	lw t1, 40(sp)
	mul a0, t0, t1
	sw a0, 56(sp)
	slli a0, a0, 2
	jal ra, malloc
	beq zero, a0, malloc_error
	sw a0, 60(sp)

	mv a6, a0
	lw a0, 32(sp)
	lw a1, 24(sp)
	lw a2, 28(sp)
	lw a3, 52(sp)
	lw a4, 12(sp)
	lw a5, 40(sp)
	jal ra, matmul

	lw a1, 4(sp)
	lw a0, 16(a1)
	lw a1, 60(sp)
	lw a2, 24(sp)
	lw a3, 40(sp)
	jal ra, write_matrix

	lw a0, 60(sp)
	lw a1, 56(sp)
	jal ra, argmax
	sw a0, 64(sp)

	lw t0, 8(sp)
	bne t0, zero, done
	jal ra, print_int
	li a0, '\n'
	jal ra, print_char
done:
	lw a0, 20(sp)
	jal ra, free
	lw a0, 32(sp)
	jal ra, free
	lw a0, 44(sp)
	jal ra, free
	lw a0, 52(sp)
	jal ra, free
	lw a0, 60(sp)
	jal ra, free

	lw a0, 64(sp)
	lw ra, 0(sp)
	addi sp, sp, 68
	ret

malloc_error:
	li a0, 26
	j exit

error_code_31:
	li a0, 31
	j exit

print_str:
	addi sp, sp, -12
	sw ra, 0(sp)
	sw zero, 4(sp)
	sw a0, 8(sp)
loop:
	lw a0, 8(sp)
	lw a1, 4(sp)
	addi t0, a1, 1
	sw t0, 4(sp)
	add a0, a0, a1
	lb a0, 0(a0)
	beq a0, zero, print_str_done
	jal ra, print_char
	j loop
print_str_done:
	lw ra, 0(sp)
	addi sp, sp, 12
	ret







	# Read pretrained m1


	# Read input matrix


	# Compute h = matmul(m0, input)


	# Compute h = relu(h)


	# Compute o = matmul(m1, h)


	# Write output matrix o


	# Compute and return argmax(o)


	# If enabled, print argmax(o) and newline


	ret
