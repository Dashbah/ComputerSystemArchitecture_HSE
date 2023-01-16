	.file	"getResult.c"          # название файла, можно удалить 
	.intel_syntax noprefix         # используемый синтаксис
	.text                          # начало секции
	.globl	getResult              # объявление глобальной переменной (функции)
	.type	getResult, @function   # и ее характеристик
getResult:
	endbr64
	push	rbp                    # пролог
	mov	rbp, rsp               #
	mov	DWORD PTR -4[rbp], 0   # подготовка места под переменную i
	jmp	.L2
.L7:
	mov	eax, DWORD PTR -4[rbp]      # кладем в регистр eax значение на стеке i
	cdqe
	lea	rdx, str[rip]               # помещаем в регистр текущий элемент строки 
	movzx	eax, BYTE PTR [rax+rdx]     # 
	cmp	al, 40                      # сравнение текущего элемента строки с '('
	jne	.L3                         # если не равно - прыгаем в .L3
	mov	eax, DWORD PTR checker[rip] # помещаем в регистр переменную чекер 
	add	eax, 1                      # добавляем к ней 1
	mov	DWORD PTR checker[rip], eax  # (возвращаем обратно на стек)
	jmp	.L4
.L3:
	mov	eax, DWORD PTR -4[rbp]        # кладем в регистр eax значение на стеке i
	cdqe
	lea	rdx, str[rip]                 # помещаем в регистр текущий элемент строки
	movzx	eax, BYTE PTR [rax+rdx]
	cmp	al, 41                        # сравнение текущего элемента строки с ')'
	jne	.L4
	mov	eax, DWORD PTR checker[rip]   # помещаем в регистр переменную чекер 
	sub	eax, 1                        # отнимаем от нее 1
	mov	DWORD PTR checker[rip], eax   # подготовка регистра для строки test
	mov	eax, DWORD PTR checker[rip]
	test	eax, eax                      # проверка на положительность 
	jns	.L9                           # jns "stores the address of the next instruction after it was called"
	mov	eax, 0
	jmp	.L6
.L9:
	nop                                   # команда "ничего не делай"
.L4:
	add	DWORD PTR -4[rbp], 1          # прибавляем 1 к переменной цикла i
.L2:
	mov	eax, DWORD PTR -4[rbp]        # кладем в регистр eax значение на стеке i
	cdqe
	lea	rdx, str[rip]                 # помещаем в регистр текущий элемент строки
	movzx	eax, BYTE PTR [rax+rdx]
	test	al, al                        
	jne	.L7
	mov	eax, DWORD PTR checker[rip]
	test	eax, eax
	jle	.L8
	mov	eax, 0
	jmp	.L6
.L8:       
	mov	eax, 1                       # return 1
.L6:
	pop	rbp                           # эпилог
	ret
	.size	getResult, .-getResult        # ненужная метаинформация
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
