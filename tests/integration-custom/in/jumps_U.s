j text
Fibonacci:
addi a0, a0, -2
li t0, 1
li t1, 1
loop:
ble a0, zero, end
add t2, t0, t1
mv t1, t0
mv t0, t2
addi a0, a0, -1
j loop
end:
mv a0, t0
ret

text:
li a0, 10
jal ra, Fibonacci
