	.file	"main.c"         # название файла, можно удалить 
	.intel_syntax noprefix   # используемый синтаксис 
	.text                    # начало секции 
	.globl	str              # объявление глобальной переменной - строки и ее параметров 
	.bss 
	.align 32
	.type	str, @object
	.size	str, 1048576
str:
	.zero	1048576
	.globl	checker                 # объявление переменной чекера - используется для логики
	.align 4
	.type	checker, @object
	.size	checker, 4
checker:
	.zero	4
	.section	.rodata
.LC0:                                      # в .LC лежат строки, которые понадобятся в ходе работы программы
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
.LC5:
	.string	"Incorrect file name!"
.LC6:
	.string	"Incorrect input!"
.LC7:
	.string	"w"
.LC8:
	.string	"output.txt"
.LC9:
	.string	"Congrats! It is correct"
.LC10:
	.string	"Oops! It is not correct..."
.LC11:
	.string	"\"Oops! It is not correct...\""
.LC12:
	.string	"\nExecution time: "
	.text                               # начало секции 
	.globl	main                        # объявление функции main
	.type	main, @function
main:
	endbr64
	push	rbp               # пролог
	mov	rbp, rsp
	sub	rsp, 32
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	puts@PLT          # функция вывода в консоль (выводим .LC0)
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	puts@PLT          # функция вывода в консоль (выводим .LC1)
	lea	rax, .LC2[rip]
	mov	rdi, rax
	call	puts@PLT          # функция вывода в консоль (выводим .LC2)
	lea	rax, .LC3[rip]
	mov	rdi, rax
	call	puts@PLT          # функция вывода в консоль (выводим .LC3)
	lea	rax, -28[rbp]     # в  -28[rbp] лежит inputType (от 1 до 3 либо другое)
	mov	rsi, rax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT       # читаем inputType
	mov	eax, DWORD PTR -28[rbp]
	cmp	eax, 3                   # /switch-case по способу ввода(inputType)
	je	.L2
	cmp	eax, 3
	jg	.L3
	cmp	eax, 1
	je	.L4
	cmp	eax, 2
	je	.L5
	jmp	.L3                       # \switch-case по способу ввода(inputType)
.L4:                                  # если ввели 1, вызываем consoleInput()
	mov	eax, 0          
	call	consoleInput@PLT          
	jmp	.L6
.L5:                                  # если ввели 1, вызываем fileInput()
	mov	eax, 0
	call	fileInput@PLT
	xor	eax, 1
	test	al, al
	je	.L14
	lea	rax, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT          # если fileInput() вернул false(нет введенного файла) и заканчиваем программу
	mov	eax, 0
	jmp	.L13
.L2:                                # если ввели 3, вызываем randomGeneration()
	mov	eax, 0
	call	randomGeneration@PLT
	jmp	.L6
.L3:                            # если ввели(inputType) не 1/2/3, пишем об этом в консоль и заканчиваем программу
	lea	rax, .LC6[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	jmp	.L13
.L14:                           # команда "ничего не делай". Иногда такая команда используется что бы
	nop                     # выравнивать команды на стеке, что бы они все были кратны 4 байтам 
.L6:                            # часть кода после switch-case
	lea	rax, .LC7[rip]
	mov	rsi, rax
	lea	rax, .LC8[rip]
	mov	rdi, rax
	call	fopen@PLT       # открываем файл для записи 
	mov	QWORD PTR -16[rbp], rax
	call	clock@PLT       # записываем, сколько сейчас времени
	mov	QWORD PTR -24[rbp], rax     # -24[rbp] - переменная t - время 
	mov	DWORD PTR -8[rbp], 0    # -8[rbp] - переменная цикла i
	jmp	.L9
.L10:                          # тело цикла, считаем результат getResult()
	mov	eax, 0
	call	getResult@PLT
	mov	BYTE PTR -1[rbp], al    # в -1[rbp] лежит result (bool)
	add	DWORD PTR -8[rbp], 1
.L9:
	cmp	DWORD PTR -8[rbp], 19999   # смотрим, нужно ли закончить цикл 
	jle	.L10
	call	clock@PLT                  # цикл закончился, вычисляем время сейчас 
	sub	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR -24[rbp], rax
	cmp	BYTE PTR -1[rbp], 0        # сравниваем результат с 0. Если равен - последовательность неверная
	je	.L11
	lea	rax, .LC9[rip]             # если не равен, то берем из .LC9 сообщение о том, что она правильная 
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -16[rbp]    # в -16[rbp] лежит ссылка на файл 
	mov	rcx, rax                   # (не уверна насчет ссылки, но оно точно связано с файлом)
	mov	edx, 23
	mov	esi, 1
	lea	rax, .LC9[rip]
	mov	rdi, rax
	call	fwrite@PLT                 # записываем в файл результат 
	jmp	.L12
.L11:                                   # записываем в файл результат о неверной последовательности 
	lea	rax, .LC10[rip]       
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rcx, rax
	mov	edx, 28
	mov	esi, 1
	lea	rax, .LC11[rip]
	mov	rdi, rax
	call	fwrite@PLT
.L12:                            # выводим сообщение об времени исполнения
	lea	rax, .LC12[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -24[rbp]    # -24[rbp] - переменная t - время 
	mov	esi, eax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT                
	mov	eax, 0
.L13:
	leave                # эпилог
	ret
	.size	main, .-main           # дальше ненужная метаинформация 
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
