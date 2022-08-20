.globl abs

.text
# =================================================================
# FUNCTION: Given an int return its absolute value.
# Arguments:
#   a0 (int) is input integer
# Returns:
#   a0 (int) the absolute value of the input
# =================================================================
abs:
	# Prologue

	# PASTE HERE
	blt zero, a0, done

	sub a0, zero, a0

done:
	ret
	# Epilogue

	ret
