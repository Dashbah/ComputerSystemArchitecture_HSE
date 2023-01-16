	.file	"result.c"
	.intel_syntax noprefix
	.text
	.globl	func
	.type	func, @function
func:
	endbr64			# в этой функции происходит просто подсчет a + b / (x * x * x * x);
	push	rbp
	mov	rbp, rsp
	movsd	QWORD PTR -8[rbp], xmm0		# -8[rbp] - значение х
	movsd	xmm1, QWORD PTR b[rip]		# ~= mov для целых
	movsd	xmm0, QWORD PTR -8[rbp]
	mulsd	xmm0, xmm0
	mulsd	xmm0, QWORD PTR -8[rbp]		# mulsd Multiplies the source operand by destination operand
	mulsd	xmm0, QWORD PTR -8[rbp]
	divsd	xmm1, xmm0
	movsd	xmm0, QWORD PTR a[rip]
	addsd	xmm0, xmm1			# ~= add для целых
	movq	rax, xmm0
	movq	xmm0, rax
	pop	rbp
	ret
	.size	func, .-func
	.globl	getResult
	.type	getResult, @function
getResult:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 24
	pxor	xmm0, xmm0
	movsd	QWORD PTR -8[rbp], xmm0		# -16[rbp] - result
	movsd	xmm0, QWORD PTR left[rip]	
	movsd	QWORD PTR -16[rbp], xmm0	# -16[rbp] - x
	jmp	.L4				# переходим в цикл
.L7:						# if (x!=0)
	pxor	xmm0, xmm0
	ucomisd	xmm0, QWORD PTR -16[rbp]
	jp	.L9
	pxor	xmm0, xmm0
	ucomisd	xmm0, QWORD PTR -16[rbp]
	je	.L5
.L9:						# result += 0.00005 * (func(x) + func(x + 0.0001));
	mov	rax, QWORD PTR -16[rbp]
	movq	xmm0, rax
	call	func
	movsd	QWORD PTR -24[rbp], xmm0
	movsd	xmm1, QWORD PTR -16[rbp]
	movsd	xmm0, QWORD PTR .LC1[rip]
	addsd	xmm1, xmm0
	movq	rax, xmm1
	movq	xmm0, rax
	call	func
	movsd	xmm1, QWORD PTR -24[rbp]
	addsd	xmm1, xmm0
	movsd	xmm0, QWORD PTR .LC2[rip]
	mulsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR -8[rbp]
	addsd	xmm0, xmm1
	movsd	QWORD PTR -8[rbp], xmm0
.L5:
	movsd	xmm1, QWORD PTR -16[rbp]
	movsd	xmm0, QWORD PTR .LC1[rip]
	addsd	xmm0, xmm1
	movsd	QWORD PTR -16[rbp], xmm0
.L4:						# for (double x = left; x <= right - 0.0001; x += 0.0001) 
	movsd	xmm0, QWORD PTR right[rip]
	movsd	xmm1, QWORD PTR .LC1[rip]
	subsd	xmm0, xmm1
	comisd	xmm0, QWORD PTR -16[rbp]
	jnb	.L7
	movsd	xmm0, QWORD PTR -8[rbp]
	movq	rax, xmm0
	movq	xmm0, rax
	leave
	ret
	.size	getResult, .-getResult
	.globl	checker
	.type	checker, @function
checker:					# в чекере происходит только подсчет одной функции
	endbr64					# a * right - a * left + b / (3 * left * left * left) - b / (3 * right * right * right);
	push	rbp
	mov	rbp, rsp
	movsd	xmm1, QWORD PTR a[rip]
	movsd	xmm0, QWORD PTR right[rip]
	mulsd	xmm0, xmm1
	movsd	xmm2, QWORD PTR a[rip]
	movsd	xmm1, QWORD PTR left[rip]
	mulsd	xmm2, xmm1
	movapd	xmm1, xmm0
	subsd	xmm1, xmm2
	movsd	xmm0, QWORD PTR b[rip]
	movsd	xmm3, QWORD PTR left[rip]
	movsd	xmm2, QWORD PTR .LC3[rip]
	mulsd	xmm3, xmm2
	movsd	xmm2, QWORD PTR left[rip]
	mulsd	xmm3, xmm2
	movsd	xmm2, QWORD PTR left[rip]
	mulsd	xmm2, xmm3
	divsd	xmm0, xmm2
	addsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR b[rip]
	movsd	xmm3, QWORD PTR right[rip]
	movsd	xmm2, QWORD PTR .LC3[rip]
	mulsd	xmm3, xmm2
	movsd	xmm2, QWORD PTR right[rip]
	mulsd	xmm3, xmm2
	movsd	xmm2, QWORD PTR right[rip]
	mulsd	xmm2, xmm3
	divsd	xmm1, xmm2
	subsd	xmm0, xmm1
	movq	rax, xmm0
	movq	xmm0, rax
	pop	rbp
	ret
	.size	checker, .-checker
	.section	.rodata
	.align 8
.LC1:
	.long	-350469331
	.long	1058682594
	.align 8
.LC2:
	.long	-350469331
	.long	1057634018
	.align 8
.LC3:
	.long	0
	.long	1074266112
