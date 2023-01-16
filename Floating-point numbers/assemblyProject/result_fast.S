	.intel_syntax noprefix		# этот файл содержит код с максимальным использованием регистров процессора вместо стека
	.text				# работает быстрее + занимает меньше места
	.globl	func
	.type	func, @function
func:
	endbr64
	movaps	xmm1, xmm0
	mulsd	xmm1, xmm0
	mulsd	xmm1, xmm0
	mulsd	xmm1, xmm0
	movsd	xmm0, QWORD PTR b[rip]
	divsd	xmm0, xmm1
	addsd	xmm0, QWORD PTR a[rip]
	ret
	.size	func, .-func
	.globl	getResult
	.type	getResult, @function
getResult:
	endbr64
	movsd	xmm0, QWORD PTR right[rip]
	movsd	xmm6, QWORD PTR .LC1[rip]
	xorps	xmm4, xmm4
	movsd	xmm3, QWORD PTR left[rip]
	movaps	xmm7, xmm4
	movsd	xmm8, QWORD PTR .LC2[rip]
	movaps	xmm5, xmm0
	subsd	xmm5, xmm6
.L3:
	comisd	xmm5, xmm3
	jb	.L9
	ucomisd	xmm3, xmm7
	jp	.L7
	je	.L4
.L7:
	movaps	xmm0, xmm3
	call	func
	movaps	xmm2, xmm0
	movaps	xmm0, xmm3
	addsd	xmm0, xmm6
	call	func
	addsd	xmm2, xmm0
	mulsd	xmm2, xmm8
	addsd	xmm4, xmm2
.L4:
	addsd	xmm3, xmm6
	jmp	.L3
.L9:
	movaps	xmm0, xmm4
	ret
	.size	getResult, .-getResult
	.globl	checker
	.type	checker, @function
checker:
	endbr64
	movsd	xmm6, QWORD PTR left[rip]
	movsd	xmm1, QWORD PTR .LC3[rip]
	movsd	xmm4, QWORD PTR right[rip]
	movsd	xmm3, QWORD PTR b[rip]
	movaps	xmm2, xmm6
	movsd	xmm5, QWORD PTR a[rip]
	mulsd	xmm2, xmm1
	movaps	xmm0, xmm3
	mulsd	xmm1, xmm4
	mulsd	xmm2, xmm6
	mulsd	xmm1, xmm4
	mulsd	xmm2, xmm6
	mulsd	xmm1, xmm4
	divsd	xmm0, xmm2
	movaps	xmm2, xmm5
	mulsd	xmm2, xmm4
	mulsd	xmm5, xmm6
	subsd	xmm2, xmm5
	divsd	xmm3, xmm1
	addsd	xmm0, xmm2
	subsd	xmm0, xmm3
	ret
	.size	checker, .-checker
	.section	.rodata.cst8,"aM",@progbits,8
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
