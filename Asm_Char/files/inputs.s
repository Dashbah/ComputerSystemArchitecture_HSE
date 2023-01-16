	.file	"inputs.c"       # более подробные комментарии по ассемблерному коду в файлах getResult.s и main.s
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:                               # в .LC лежат строки, которые понадобятся в ходе работы программы
	.string	"Enter your string: "
.LC1:
	.string	"%s"
	.text
	.globl	consoleInput
	.type	consoleInput, @function
consoleInput:
	endbr64
	push	rbp
	mov	rbp, rsp
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	puts@PLT        # просим пользователя ввести строку
	lea	rax, str[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT   # считываем строку
	nop
	pop	rbp
	ret
	.size	consoleInput, .-consoleInput
	.section	.rodata
.LC2:
	.string	"Enter input file name: "
.LC3:
	.string	"r"
	.text
	.globl	fileInput
	.type	fileInput, @function
fileInput:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	lea	rax, .LC2[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT     # просим пользователя ввести имя файла
	lea	rax, -64[rbp]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT    # считываем имя файла
	lea	rax, -64[rbp]
	mov	esi, 0
	mov	rdi, rax
	call	access@PLT    # проверяем, существует ли такой файл
	test	eax, eax
	jne	.L3           # прыгаем, если не существует
	lea	rax, -64[rbp]
	lea	rdx, .LC3[rip]
	mov	rsi, rdx
	mov	rdi, rax
	call	fopen@PLT       # открываем файл
	mov	QWORD PTR -8[rbp], rax     # -8[rbp] - файл ввода 
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, str[rip]
	lea	rcx, .LC1[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT   # считываем строку из файла
	mov	eax, 1
	jmp	.L5
.L3:
	mov	eax, 0
.L5:
	leave
	ret
	.size	fileInput, .-fileInput
	.section	.rodata
.LC4:
	.string	"Your string: "
	.text
	.globl	randomGeneration
	.type	randomGeneration, @function
randomGeneration:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, -16[rbp]    # -16[rbp] - time_t t
	mov	rdi, rax
	call	time@PLT      
	mov	edi, eax
	call	srand@PLT     
	call	rand@PLT      # здесь начинается подсчет размера строки 
	movsx	rdx, eax
	imul	rdx, rdx, 1374389535
	shr	rdx, 32
	sar	edx, 4
	mov	ecx, eax
	sar	ecx, 31
	sub	edx, ecx
	imul	ecx, edx, 50       # здесь заканчивается подсчет размера строки 
	sub	eax, ecx
	mov	edx, eax
	lea	eax, 3[rdx]
	mov	DWORD PTR -8[rbp], eax     # -8[rbp] - длина строки n
	mov	DWORD PTR -4[rbp], 0       # -4[rbp] - переменная цикла i
	jmp	.L7
.L8:
	call	rand@PLT         # генерируем рандом, дальше идет арифметика по подсчету нового символа
	mov	edx, eax
	movsx	rax, edx
	imul	rax, rax, 715827883
	shr	rax, 32
	sar	eax, 2
	mov	esi, edx
	sar	esi, 31
	sub	eax, esi
	mov	ecx, eax
	mov	eax, ecx
	add	eax, eax
	add	eax, ecx
	sal	eax, 3
	mov	ecx, edx
	sub	ecx, eax
	mov	eax, ecx
	add	eax, 33
	mov	ecx, eax
	mov	eax, DWORD PTR -4[rbp]
	cdqe
	lea	rdx, str[rip]
	mov	BYTE PTR [rax+rdx], cl
	add	DWORD PTR -4[rbp], 1     # увеличиваем счетчик i
.L7:
	mov	eax, DWORD PTR -4[rbp]
	cmp	eax, DWORD PTR -8[rbp]      # сравниваем n c i
	jl	.L8                         # если меньше - прыгаем. Иначе - выходим из цикла и выводим сообщения 
	lea	rax, .LC4[rip]
	mov	rdi, rax
	call	puts@PLT
	lea	rax, str[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	edi, 10
	call	putchar@PLT
	nop
	leave                               # эпилог
	ret
	.size	randomGeneration, .-randomGeneration    # дальше ненужная метаинформация 
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5        
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
