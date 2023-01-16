	.file	"main.c" # название файла, можно убрать
	.intel_syntax noprefix # используем синтаксис интел
	.text            # начало секции
	.globl	ARRAY_A  # объявление глобальной переменной ARRAY_A
	.bss             # с указанием ее параметров
	.align 32
	.type	ARRAY_A, @object
	.size	ARRAY_A, 4194304
ARRAY_A:
	.zero	4194304 # объявление глобальной переменной ARRAY_B
	.globl	ARRAY_B
	.align 32
	.type	ARRAY_B, @object
	.size	ARRAY_B, 4194304
ARRAY_B:
	.zero	4194304 # / объявление глобальной переменной size_B
	.globl	size_B
	.align 4
	.type	size_B, @object
	.size	size_B, 4
size_B:
	.zero	4
	.section	.rodata  # Переходим в секцию .rodata
.LC0:
	.string	"r"   # .LC0: "r"
.LC1:
	.string	"input.txt"  # .LC1: "input.txt"
.LC2:
	.string	"%d"    # .LC2: "%d"
	.text        # начало секции
	.globl	createFromFile  # / Объявляем и экспортируем вовне символ
                            # \ `createFromFile`
	.type	createFromFile, @function  # И сразу отметим, что это функция
createFromFile:              # Теперь метка `createFromFile:`, именно её мы и экспортируем
	endbr64
	push	rbp              # /Стандартный пролог
	mov	rbp, rsp             # \
	sub	rsp, 32              # выделяем необходимый размер стека
	lea	rax, .LC0[rip]       # rax := rip[.LC0] — строчка "r"
	mov	rsi, rax             # можно сразу в rsi записать .LC0[rip] (изм2)
	lea	rax, .LC1[rip]       # анологично: можно сразу записать .LC1[rip] в rdi
	mov	rdi, rax             # .LC1: "input.txt"
	call	fopen@PLT        # вызов функции открытия файла
	mov	QWORD PTR -16[rbp], rax  # созраняем аргумент на стек (rax := rip[.LC0] — строчка "r")
	lea	rdx, -20[rbp]            # n
	mov	rax, QWORD PTR -16[rbp]  # в rax записывается строчка "r"
	lea	rcx, .LC2[rip]           # в rсx записывается .LC2: "%d"
	mov	rsi, rcx                 # анологично: можно сразу записать LC2: "%d" в rsi
	mov	rdi, rax                 # в rdi записывается строчка "r"
	mov	eax, 0                   # eax := 0
	call	__isoc99_fscanf@PLT  # fscanf(input, "%d", &n)
	mov	DWORD PTR -4[rbp], 0     # rbp[-4] := 0 — счетчик цикла
	jmp	.L2                      # переход к метке .L2: ниже по коду, там проверка условия цикла
.L3:
	lea	rdx, -24[rbp]            # rdx := rbp[-24]
	mov	rax, QWORD PTR -16[rbp]  # в rax записывается строчка "r"
	lea	rcx, .LC2[rip]           # в rсx записывается .LC2: "%d"
	mov	rsi, rcx                 # /(изм3)
	mov	rdi, rax                 # \
	mov	eax, 0                   # eax := 0
	call	__isoc99_fscanf@PLT  # fscanf(input, "%d", &num)
	mov	eax, DWORD PTR -24[rbp]  # eax := rbp[-24]
	mov	edx, DWORD PTR -4[rbp]   # rbp[-4] — счетчик цикла
	movsx	rdx, edx
	lea	rcx, 0[0+rdx*4]          # rcx := rdx * 4 вычисляет адрес (rax*4)[0], который равен rax*4
	lea	rdx, ARRAY_A[rip]        # rdx := &rip[ARRAY] — адрес начала массива
	mov	DWORD PTR [rcx+rdx], eax  # \ [rcx + rdx] := eax — наконец, записать в rdx[rcx] := eax
	add	DWORD PTR -4[rbp], 1     # rbp[-4] (счетчик) += 1
.L2:
	mov	eax, DWORD PTR -20[rbp]  # eax := rbp[-20]
	cmp	DWORD PTR -4[rbp], eax   # сравнить rbp[-4] и eax (это счетчик цикла и N)
	jl	.L3                      # если меньше, то перейти к .L3: (иначе выйти из цикла)
	nop
	nop
	leave               # Эпилог
	ret                 # Эпилог
	.size	createFromFile, .-createFromFile  # мета-информация, она не нужна
	.globl	getResult               # / Объявляем и экспортируем вовне символ
                                    # \ `getResult`
	.type	getResult, @function    # И сразу отметим, что это функция
getResult:
	endbr64
	push	rbp                     # /Стандартный пролог
	mov	rbp, rsp                    # /Стандартный пролог
	mov	DWORD PTR -20[rbp], edi     # rbp[-20] := edi — это первый аргумент, `n`
	mov	DWORD PTR -4[rbp], 0        # average = 0
	mov	DWORD PTR -8[rbp], 0        # i = 0
	jmp	.L5                         # переход к метке .L5:
.L6:
	mov	eax, DWORD PTR -8[rbp]     # eax := i
	cdqe
	lea	rdx, 0[0+rax*4]            # rdx := rax * 4 вычисляет адрес (rax*4)[0], который равен rax*4
	lea	rax, ARRAY_A[rip]           # rax := &rip[ARRAY_A] — адрес начала массива
	mov	eax, DWORD PTR [rdx+rax]    # eax := *(rdx + rax)
	add	DWORD PTR -4[rbp], eax      # average += eax
	add	DWORD PTR -8[rbp], 1        # ++i
.L5:
	mov	eax, DWORD PTR -8[rbp]   # eax := i
	cmp	eax, DWORD PTR -20[rbp]  #
	jl	.L6                      # go to .L6
	mov	eax, DWORD PTR -4[rbp]   # rbp[-4] - average
	cdq                          # sign-extend
	idiv	DWORD PTR -20[rbp]
	mov	DWORD PTR -4[rbp], eax   # rbp[-4] := eax
	mov	eax, DWORD PTR ARRAY_A[rip]  # eax := &rip[ARRAY_A] — адрес начала массива
	mov	DWORD PTR -12[rbp], eax      # rbp[-12] %= eax (sum)
	mov	DWORD PTR size_B[rip], 1    # size_B := 1
	mov	DWORD PTR -8[rbp], 0        # i := 0
	jmp	.L7                        # jump to .L7
.L9:
	mov	eax, DWORD PTR -8[rbp]   # здесь все аналогично с предыдущими строками, ничего нового
	cdqe
	lea	rcx, 0[0+rax*4]
	lea	rdx, ARRAY_B[rip]
	mov	eax, DWORD PTR -12[rbp]
	mov	DWORD PTR [rcx+rdx], eax
	mov	eax, DWORD PTR -8[rbp]
	add	eax, 1
	cdqe
	lea	rdx, 0[0+rax*4]
	lea	rax, ARRAY_A[rip]
	mov	eax, DWORD PTR [rdx+rax]
	add	DWORD PTR -12[rbp], eax
	mov	eax, DWORD PTR size_B[rip]
	add	eax, 1
	mov	DWORD PTR size_B[rip], eax
	add	DWORD PTR -8[rbp], 1
.L7:
	mov	eax, DWORD PTR -8[rbp]
	cdqe
	lea	rdx, 0[0+rax*4]
	lea	rax, ARRAY_A[rip] # начало массива
	mov	eax, DWORD PTR [rdx+rax]
	cmp	DWORD PTR -4[rbp], eax # сравниваем ARRAY_A[i] с average
	jl	.L8   # если больше - выходим из цикла, нет - проверяем второе условие
	mov	eax, DWORD PTR -8[rbp]
	cmp	eax, DWORD PTR -20[rbp] # сравниваем i с n
	jl	.L9
.L8:
	mov	eax, DWORD PTR -8[rbp]
	cdqe
	lea	rcx, 0[0+rax*4]
	lea	rdx, ARRAY_B[rip]
	mov	eax, DWORD PTR -12[rbp]
	mov	DWORD PTR [rcx+rdx], eax
	nop
	pop	rbp                 # Стандартный эпилог
	ret                     # Стандартный эпилог
	.size	getResult, .-getResult # metainf
	.section	.rodata  # Переходим в секцию .rodata
	.align 8             # Выравнивание
.LC3:               # .LC3: "for file input type f, for random : r"
	.string	"for file input type f, for random : r"
.LC4:               # .LC4: "%c"
	.string	"%c"
.LC5:               # .LC5: "incorrect command"
	.string	"incorrect command"
.LC6:               # .LC6: "w"
	.string	"w"
.LC7:               # .LC7: "output.txt"
	.string	"output.txt"
	.text           # начало секции
	.globl	main    # / Объявляем и экспортируем вовне символ 'main'
	.type	main, @function  # И сразу отметим, что это функция
main:
	endbr64
	push	rbp   # / стандартный пролог
	mov	rbp, rsp  # |
	sub	rsp, 48   # \ выделение памяти
	lea	rax, -32[rbp] # time_t t
	mov	rdi, rax
	call	time@PLT # вызов функции
	mov	edi, eax
	call	srand@PLT # вызов функции
	lea	rax, .LC3[rip]  # rsi := rip[.LC3] — строчка "w"
	mov	rdi, rax
	mov	eax, 0          # обнуление eax
	call	printf@PLT  # вызов функции
	mov	edi, 10   # видимо из-за этого у меня выводится 10
    call	putchar@PLT
	lea	rax, -37[rbp]
	mov	rsi, rax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT  # считываем type ('f'/'r')
	movzx	eax, BYTE PTR -37[rbp]
	cmp	al, 102 # начинается разветвление по типу желаемого заполнения массива
	jne	.L11
	lea	rax, .LC0[rip]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -16[rbp], rax
	lea	rdx, -36[rbp]
	mov	rax, QWORD PTR -16[rbp]
	lea	rcx, .LC2[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT
	mov	DWORD PTR -8[rbp], 0
	jmp	.L12
.L13:
	lea	rdx, -44[rbp]
	mov	rax, QWORD PTR -16[rbp]
	lea	rcx, .LC2[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf@PLT
	mov	eax, DWORD PTR -44[rbp]
	mov	edx, DWORD PTR -8[rbp]
	movsx	rdx, edx
	lea	rcx, 0[0+rdx*4]
	lea	rdx, ARRAY_A[rip]
	mov	DWORD PTR [rcx+rdx], eax
	add	DWORD PTR -8[rbp], 1
.L12:
	mov	eax, DWORD PTR -36[rbp]
	cmp	DWORD PTR -8[rbp], eax
	jl	.L13
	jmp	.L14
.L11:
	movzx	eax, BYTE PTR -37[rbp]
	cmp	al, 114
	jne	.L15
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
	lea	eax, 5[rcx]
	mov	DWORD PTR -36[rbp], eax
	mov	eax, DWORD PTR -36[rbp]
	mov	edi, eax
	call	createRandomArray@PLT
	jmp	.L14
.L15:
	lea	rax, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 1
	jmp	.L19
.L14:
	mov	esi, 10
	lea	rax, .LC2[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, DWORD PTR -36[rbp]
	mov	edi, eax
	call	getResult
	lea	rax, .LC6[rip]
	mov	rsi, rax
	lea	rax, .LC7[rip]
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	DWORD PTR -4[rbp], 0
	jmp	.L17
.L18:
	mov	eax, DWORD PTR -4[rbp]
	cdqe
	lea	rdx, 0[0+rax*4]
	lea	rax, ARRAY_B[rip]
	mov	edx, DWORD PTR [rdx+rax]
	mov	rax, QWORD PTR -24[rbp]
	lea	rcx, .LC2[rip]
	mov	rsi, rcx
	mov	rdi, rax
	mov	eax, 0
	call	fprintf@PLT
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 32
	call	fputc@PLT
	add	DWORD PTR -4[rbp], 1
.L17: # вывод элементов из ARRAY_B
	mov	eax, DWORD PTR size_B[rip]
	cmp	DWORD PTR -4[rbp], eax # сравниваем i с size_B
	jl	.L18
	mov	eax, 0
.L19:
	leave  # / эпилог
	ret    # \
	.size	main, .-main  # / ненужная метаинформация
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
4:                  # \ ненужная метаинформация
