global isr_stub_table
global interrupt_handler_asm

extern exception_handler
extern interrupt_handler

%macro isr_err_stub 1

isr_stub_%+%1:
	call exception_handler
	iretq

%endmacro

%macro isr_no_err_stub 1

isr_stub_%+%1:
	call exception_handler
	iretq

%endmacro

%macro irq_no_err_stub 1

irq_stub_%+%1:
	call interrupt_handler
	iretq

%endmacro

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

irq_no_err_stub 100

section .text

isr_stub_table:
	%assign i 0
	%rep 32
		dd isr_stub_%+i
		%assign i i+1
	%endrep

	%assign i 100
	dd irq_stub_%+i
