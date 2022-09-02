addi t0, zero, 1
beq t0, zero, a
addi t1, t1, 1
a:
bne t0, zero, b
addi t1, t1, 1
b:
bge t0, zero, c
addi t1, t1, 1
c:
addi t2, zero, -1
bgeu t0, t2, d
addi t1, t1, 1
d:
blt t0, zero, e
addi t1, t1, 1
e:
bltu t0, t2, f
addi t1, t1, 1
f:
bne t0, zero, g
addi t1, t1, 1
g:
