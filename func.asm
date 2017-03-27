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
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -4
	sw $a1, 0($sp)
	addi $sp, $sp, -4
	sw $a2, 0($sp)
	addi $sp, $sp, -4
	sw $a3, 0($sp)
	addi $sp, $sp, -4
	sw $s0, 0($sp)
	addi $sp, $sp, -4
	sw $s1, 0($sp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	add $t9, $s0, 0
	addi $a0, $t9, 0
	jal Sum
	addi $t9, $v0, 0
	lw $ra,0($sp)
	addi $sp, $sp, 4
	lw $v0,0($sp)
	addi $sp, $sp, 4
	lw $s1,0($sp)
	addi $sp, $sp, 4
	lw $s0,0($sp)
	addi $sp, $sp, 4
	lw $a3,0($sp)
	addi $sp, $sp, 4
	lw $a2,0($sp)
	addi $sp, $sp, 4
	lw $a1,0($sp)
	addi $sp, $sp, 4
	lw $a0,0($sp)
	addi $sp, $sp, 4
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
IF0:
	add $t9, $a0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	li $t9, 0
	lw $t8,0($sp)
	addi $sp, $sp, 4
	bne $t8, $t9, ELSE0
	add $t9, $a0, 0
	add $v0, $t9, 0
	j ENDIF0
	ELSE0:
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -4
	sw $a1, 0($sp)
	addi $sp, $sp, -4
	sw $a2, 0($sp)
	addi $sp, $sp, -4
	sw $a3, 0($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	add $t9, $a0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	li $t9, 1
	lw $t8,0($sp)
	addi $sp, $sp, 4
	sub $t9, $t8, $t9
	addi $a0, $t9, 0
	jal Sum
	addi $t9, $v0, 0
	lw $ra,0($sp)
	addi $sp, $sp, 4
	lw $v0,0($sp)
	addi $sp, $sp, 4
	lw $t0,0($sp)
	addi $sp, $sp, 4
	lw $a3,0($sp)
	addi $sp, $sp, 4
	lw $a2,0($sp)
	addi $sp, $sp, 4
	lw $a1,0($sp)
	addi $sp, $sp, 4
	lw $a0,0($sp)
	addi $sp, $sp, 4
	add $t0, $t9, 0
	add $t9, $t0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	add $t9, $a0, 0
	lw $t8,0($sp)
	addi $sp, $sp, 4
	add $t9, $t8, $t9
	add $v0, $t9, 0
ENDIF0:
	jr $ra
