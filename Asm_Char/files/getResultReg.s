	.intel_syntax noprefix
	.text
	.globl	getResult
	.type	getResult, @function
getResult:
	endbr64
	push	rbp
	mov	rbp, rsp
	# DWORD PTR -4[rbp] -> ebx (32bit)
	mov	ebx, 0
	jmp	.L2
.L7:
	mov	eax, ebx
	cdqe
	lea	rdx, str[rip]
	movzx	eax, BYTE PTR [rax+rdx]
	cmp	al, 40
	jne	.L3
	mov	eax, DWORD PTR checker[rip]
	add	eax, 1
	mov	DWORD PTR checker[rip], eax
	jmp	.L4
.L3:
	mov	eax, ebx
	cdqe
	lea	rdx, str[rip]
	movzx	eax, BYTE PTR [rax+rdx]
	cmp	al, 41
	jne	.L4
	mov	eax, DWORD PTR checker[rip]
	sub	eax, 1
	mov	DWORD PTR checker[rip], eax
	mov	eax, DWORD PTR checker[rip]
	test	eax, eax
	jns	.L9
	mov	eax, 0
	jmp	.L6
.L9:
	nop
.L4:
	add	ebx, 1
.L2:
	mov	eax, ebx
	cdqe
	lea	rdx, str[rip]
	movzx	eax, BYTE PTR [rax+rdx]
	test	al, al
	jne	.L7
	mov	eax, DWORD PTR checker[rip]
	test	eax, eax
	jle	.L8
	mov	eax, 0
	jmp	.L6
.L8:
	mov	eax, 1
.L6:
	pop	rbp
	ret
