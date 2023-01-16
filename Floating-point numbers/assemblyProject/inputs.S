	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:					# в .LC лежат строки, которые понадобятся в ходе работы программы
	.string	"a, b, left, right: "
.LC1:
	.string	"%lf"
	.text
	.globl	consoleInput
	.type	consoleInput, @function
consoleInput:
	endbr64
	push	rbp
	mov	rbp, rsp
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	puts@PLT         # printf("a, b, left, right: \n");
	lea	rax, a[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT   # здесь мы считываем 4 параметра из консоли
	lea	rax, b[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT
	lea	rax, left[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT
	lea	rax, right[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT
	movsd	xmm1, QWORD PTR left[rip] # /проверка корректности введенных данных left и right
	pxor	xmm0, xmm0
	comisd	xmm0, xmm1                # функция сравнения чисел с плавающей точкой 
	jb	.L2
	movsd	xmm0, QWORD PTR right[rip]
	pxor	xmm1, xmm1
	comisd	xmm0, xmm1
	jnb	.L4			  # \проверка корректности введенных данных left и right
.L2:
	movsd	xmm0, QWORD PTR left[rip]
	movsd	xmm1, QWORD PTR right[rip]
	comisd	xmm0, xmm1
	jb	.L9
.L4:
	mov	eax, 0
	jmp	.L7
.L9:
	mov	eax, 1
.L7:					# выход из функции
	pop	rbp
	ret
	.size	consoleInput, .-consoleInput
	.section	.rodata
.LC3:
	.string	"Enter input file name: "
.LC4:
	.string	"%s"
.LC5:
	.string	"r"
	.text
	.globl	fileInput
	.type	fileInput, @function
fileInput:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	lea	rax, .LC3[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT		# просим пользователя ввести имя файла
	lea	rax, -64[rbp]
	mov	rsi, rax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT      # считываем имя файла
	lea	rax, -64[rbp]
	mov	esi, 0
	mov	rdi, rax
	call	access@PLT		# проверяем, существует ли такой файл
	test	eax, eax
	jne	.L11			# прыгаем, если не существует
	lea	rax, -64[rbp]		
	lea	rdx, .LC5[rip]
	mov	rsi, rdx
	mov	rdi, rax
	call	fopen@PLT		 # открываем файл
	mov	QWORD PTR -8[rbp], rax   # -8[rbp] - файл ввода
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, a[rip]
	lea	rcx, .LC1[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT      # функция считывания параметров из файла
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, b[rip]
	lea	rcx, .LC1[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, left[rip]
	lea	rcx, .LC1[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, right[rip]
	lea	rcx, .LC1[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT
	mov	eax, 1
	jmp	.L13
.L11:
	mov	eax, 0
.L13:
	leave
	ret
	.size	fileInput, .-fileInput
	.section	.rodata
.LC7:
	.string	"Your parameters: "
.LC8:
	.string	"a: "
.LC9:
	.string	" b: "
.LC10:
	.string	" left: "
.LC11:
	.string	" right: "
	.text
	.globl	randomGeneration
	.type	randomGeneration, @function
randomGeneration:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, -8[rbp]		# -8[rbp] - time_t t
	mov	rdi, rax
	call	time@PLT
	mov	edi, eax
	call	srand@PLT               # дальше происходит генерация параметров
	call	rand@PLT
	mov	edx, eax
	movsx	rax, edx
	imul	rax, rax, 1717986919
	shr	rax, 32
	sar	eax, 3
	mov	esi, edx
	sar	esi, 31
	sub	eax, esi
	mov	ecx, eax
	mov	eax, ecx
	sal	eax, 2
	add	eax, ecx
	sal	eax, 2
	mov	ecx, edx
	sub	ecx, eax
	lea	eax, -10[rcx]
	pxor	xmm0, xmm0
	cvtsi2sd	xmm0, eax
	movsd	QWORD PTR a[rip], xmm0
	call	rand@PLT
	mov	edx, eax
	movsx	rax, edx
	imul	rax, rax, 1717986919
	shr	rax, 32
	sar	eax, 3
	mov	esi, edx
	sar	esi, 31
	sub	eax, esi
	mov	ecx, eax
	mov	eax, ecx
	sal	eax, 2
	add	eax, ecx
	sal	eax, 2
	mov	ecx, edx
	sub	ecx, eax
	lea	eax, -10[rcx]
	pxor	xmm0, xmm0
	cvtsi2sd	xmm0, eax
	movsd	QWORD PTR b[rip], xmm0
	call	rand@PLT
	movsx	rdx, eax
	imul	rdx, rdx, 1717986919
	shr	rdx, 32
	sar	edx
	mov	esi, eax
	sar	esi, 31
	sub	edx, esi
	mov	ecx, edx
	mov	edx, ecx
	sal	edx, 2
	add	edx, ecx
	sub	eax, edx
	mov	ecx, eax
	pxor	xmm1, xmm1
	cvtsi2sd	xmm1, ecx
	movsd	xmm0, QWORD PTR .LC6[rip]
	addsd	xmm0, xmm1
	movsd	QWORD PTR left[rip], xmm0
	call	rand@PLT
	mov	edx, eax
	movsx	rax, edx
	imul	rax, rax, 1717986919
	shr	rax, 32
	sar	eax, 2
	mov	esi, edx
	sar	esi, 31
	sub	eax, esi
	mov	ecx, eax
	mov	eax, ecx
	sal	eax, 2
	add	eax, ecx
	add	eax, eax
	mov	ecx, edx
	sub	ecx, eax
	pxor	xmm1, xmm1
	cvtsi2sd	xmm1, ecx
	movsd	xmm0, QWORD PTR left[rip]
	addsd	xmm0, xmm1
	movsd	QWORD PTR right[rip], xmm0
	lea	rax, .LC7[rip]
	mov	rdi, rax
	call	puts@PLT
	lea	rax, .LC8[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT			# выводим сгенерированные параметры в консоль
	mov	rax, QWORD PTR a[rip]
	movq	xmm0, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	lea	rax, .LC9[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR b[rip]
	movq	xmm0, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	lea	rax, .LC10[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR left[rip]
	movq	xmm0, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	lea	rax, .LC11[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR right[rip]
	movq	xmm0, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	mov	edi, 10
	call	putchar@PLT
	nop
	leave
	ret
	.size	randomGeneration, .-randomGeneration
	.section	.rodata
	.align 8
.LC6:
	.long	1202590843
	.long	1065646817
