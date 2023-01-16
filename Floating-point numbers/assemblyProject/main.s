	.intel_syntax noprefix
	.text
	.globl	a
	.bss
	.align 8
	.type	a, @object
	.size	a, 8
a:				# объявление глобальных переменных
	.zero	8
	.globl	b
	.align 8
	.type	b, @object
	.size	b, 8
b:
	.zero	8
	.globl	left
	.align 8
	.type	left, @object
	.size	left, 8
left:
	.zero	8
	.globl	right
	.align 8
	.type	right, @object
	.size	right, 8
right:
	.zero	8
	.section	.rodata
.LC0:						# в .LC лежат строки, которые понадобятся в ходе работы программы
	.string	"Choose input type: "
.LC1:
	.string	"For console input enter 1 "
.LC2:
	.string	"For file input enter 2 "
	.align 8
.LC3:
	.string	"For random generation enter 3 "
.LC4:
	.string	"%d"
	.align 8
.LC5:
	.string	"Incorrect limits of integration!"
.LC6:
	.string	"Incorrect file name!"
.LC7:
	.string	"Incorrect input!"
.LC8:
	.string	"w"
.LC9:
	.string	"output.txt"
.LC10:
	.string	"Result: "
.LC11:
	.string	"%f"
.LC12:
	.string	"\nExpected result: "
.LC13:
	.string	"\nResult: "
.LC14:
	.string	"\nExecution time: "
	.text
	.globl	main
	.type	main, @function
main:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	puts@PLT		# вывод инструкций в консоль
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	puts@PLT
	lea	rax, .LC2[rip]
	mov	rdi, rax
	call	puts@PLT
	lea	rax, .LC3[rip]
	mov	rdi, rax
	call	puts@PLT
	lea	rax, -44[rbp]		# в  -44[rbp] лежит inputType (от 1 до 3 либо другое)
	mov	rsi, rax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT	# читаем inputType
	mov	eax, DWORD PTR -44[rbp]		# /switch-case по способу ввода(inputType)
	cmp	eax, 3
	je	.L2
	cmp	eax, 3
	jg	.L3
	cmp	eax, 1
	je	.L4
	cmp	eax, 2
	je	.L5
	jmp	.L3
.L4:					 # если ввели 1, вызываем consoleInput()
	mov	eax, 0
	call	consoleInput@PLT
	xor	eax, 1
	test	al, al
	je	.L13
	lea	rax, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	jmp	.L12
.L5:				# если ввели 2, вызываем fileInput()
	mov	eax, 0
	call	fileInput@PLT
	xor	eax, 1
	test	al, al
	je	.L14
	lea	rax, .LC6[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	jmp	.L12
.L2:				# если ввели 3, вызываем randomGeneration()
	mov	eax, 0
	call	randomGeneration@PLT
	jmp	.L8
.L3:				# если ввели(inputType) не 1/2/3, пишем об этом в консоль и заканчиваем программу
	lea	rax, .LC7[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	jmp	.L12
.L13:
	nop
	jmp	.L8
.L14:
	nop
.L8:
	lea	rax, .LC8[rip]
	mov	rsi, rax
	lea	rax, .LC9[rip]
	mov	rdi, rax
	call	fopen@PLT		# открываем файл для записи 
	mov	QWORD PTR -24[rbp], rax
	call	clock@PLT		# записываем, сколько сейчас времени
	mov	QWORD PTR -32[rbp], rax    # -32[rbp] - переменная t - время 
	mov	DWORD PTR -12[rbp], 0      # -12[rbp] - переменная цикла i
	jmp	.L10
.L11:
	mov	eax, 0
	call	getResult@PLT
	movq	rax, xmm0
	mov	QWORD PTR -8[rbp], rax	    # в -8[rbp] лежит result (bool)
	add	DWORD PTR -12[rbp], 1
.L10:
	cmp	DWORD PTR -12[rbp], 19		# смотрим, нужно ли закончить цикл 
	jle	.L11
	call	clock@PLT			# цикл закончился, вычисляем время сейчас 
	sub	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR -32[rbp], rax
	mov	eax, 0
	call	checker@PLT			# вычисляем ожидаемый результат
	movq	rax, xmm0
	mov	QWORD PTR -40[rbp], rax		# -40[rbp] - ожидаемый результат
	lea	rax, .LC10[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT			# вывод результатов
	mov	rax, QWORD PTR -8[rbp]
	movq	xmm0, rax
	lea	rax, .LC11[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	lea	rax, .LC12[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -40[rbp]
	movq	xmm0, rax
	lea	rax, .LC11[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	mov	rax, QWORD PTR -24[rbp]
	mov	rcx, rax
	mov	edx, 9
	mov	esi, 1
	lea	rax, .LC13[rip]
	mov	rdi, rax
	call	fwrite@PLT			# запись результата в файл
	mov	rdx, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR -24[rbp]
	movq	xmm0, rdx
	lea	rdx, .LC11[rip]
	mov	rsi, rdx
	mov	rdi, rax
	mov	eax, 1
	call	fprintf@PLT
	mov	rax, QWORD PTR -24[rbp]
	mov	rcx, rax
	mov	edx, 18
	mov	esi, 1
	lea	rax, .LC12[rip]
	mov	rdi, rax
	call	fwrite@PLT
	mov	rdx, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -24[rbp]
	movq	xmm0, rdx
	lea	rdx, .LC11[rip]
	mov	rsi, rdx
	mov	rdi, rax
	mov	eax, 1
	call	fprintf@PLT
	lea	rax, .LC14[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -32[rbp]
	mov	esi, eax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	edi, 10
	call	putchar@PLT
	mov	eax, 0
.L12:
	leave
	ret
	.size	main, .-main
