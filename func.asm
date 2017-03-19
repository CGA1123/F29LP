	.data
sinp:	.asciiz "INPUT> "

	.text
main:
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	li $v0, 4
	la $a0, sinp
	syscall
	li $v0, 5
	syscall
	add $s0, $v0, 0
	lw $v0,0($sp)
	addi $sp, $sp, 4
	lw $a0,0($sp)
	addi $sp, $sp, 4
	add $t9, $s0, 0
	addi $a0, $t9, 0
	jal Sum
	addi $t9, $v0, 0
	add $s1, $t9, 0
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	add $t9, $s1, 0
	add $a0, $t9, 0
	li $v0, 1
	syscall
	li $a0, 0xA
	li $v0, 11
	syscall
	lw $v0,0($sp)
	addi $sp, $sp, 4
	lw $a0,0($sp)
	addi $sp, $sp, 4
	li $v0, 10
	syscall

Sum:
	li $t9, 0
	add $v0, $t9, 0
LOOP0:
	li $t9, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	add $t9, $a0, 0
	lw $t8,0($sp)
	addi $sp, $sp, 4
	bge $t8, $t9, ENDLOOP0
	add $t9, $v0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	add $t9, $a0, 0
	lw $t8,0($sp)
	addi $sp, $sp, 4
	add $t9, $t8, $t9
	add $v0, $t9, 0
	add $t9, $a0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	li $t9, 1
	lw $t8,0($sp)
	addi $sp, $sp, 4
	sub $t9, $t8, $t9
	add $a0, $t9, 0
	j LOOP0
ENDLOOP0:
	jr $ra
