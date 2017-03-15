	.data
sinp:	.asciiz "INPUT> "

	.text
main:
	li $v0, 4
	la $a0, sinp
	syscall
	li $v0, 5
	syscall
	add $s0, $v0, 0
	li $v0, 4
	la $a0, sinp
	syscall
	li $v0, 5
	syscall
	add $s1, $v0, 0
IF0:
	add $t9, $s0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	add $t9, $s1, 0
	lw $t8,0($sp)
	addi $sp, $sp, 4
	bge $t8, $t9, ELSE0
	add $t9, $s0, 0
	add $a0, $t9, 0
	li $v0, 1
	syscall
	li $a0, 0xA
	li $v0, 11
	syscall
	j ENDIF0
	ELSE0:
	add $t9, $s1, 0
	add $a0, $t9, 0
	li $v0, 1
	syscall
	li $a0, 0xA
	li $v0, 11
	syscall
ENDIF0:
LOOP1:
	add $t9, $s0, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	add $t9, $s1, 0
	lw $t8,0($sp)
	addi $sp, $sp, 4
	bge $t8, $t9, ENDLOOP1
	add $t9, $s1, 0
	add $a0, $t9, 0
	li $v0, 1
	syscall
	li $a0, 0xA
	li $v0, 11
	syscall
	add $t9, $s1, 0
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	li $t9, 1
	lw $t8,0($sp)
	addi $sp, $sp, 4
	sub $t9 $t8 $t9
	add $s1, $t9, 0
	j LOOP1
ENDLOOP1:
	li $v0, 10
	syscall

