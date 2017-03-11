	.data
sinp:	.asciiz "INPUT> "

	.text
main:	li $v0, 4
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
	add $t9, $s0, 0
	add $a0, $t9, 0
	li $v0, 1
	syscall
	li $a0, 0xA
	li $v0, 11
	syscall
	add $t9, $s0, 0
	add $a0, $t9, 0
	li $v0, 1
	syscall
	li $a0, 0xA
	li $v0, 11
	syscall
	li $v0, 10
	syscall

