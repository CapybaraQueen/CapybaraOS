global start

;extern long_mode_start
extern kernel_main

section .text
bits 32

start:

	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer]
	jmp gdt64.Code:long_mode_start

	hlt

check_multiboot:
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret

.no_multiboot:
	mov al, "M"
	jmp error

check_cpuid:
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	cmp eax, ecx
	je .no_cpuid
	ret

.no_cpuid:
	mov al, "C"
	jmp error

check_long_mode:
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_long_mode

	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode

	ret

.no_long_mode:
	mov al, "L"
	jmp error

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax

	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov eax, page_table_l1
	or eax, 0b11 ; present, writable
	mov [page_table_l2], eax

	mov ecx, 0 ; counter
	mov ebx, 0x00000003

.loop:
	; here we are identity mapping page table level 1 to physical memory with all present and read/write bits enabled
	mov [page_table_l1 + ecx * 8], ebx
	add ebx, 0x1000
	inc ecx
	cmp ecx, 512
	jne .loop

	ret

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_l4
	mov cr3, eax

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

error:
	; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

section .bss

align 4096

page_table_l4:
	resb 4096

page_table_l3:
	resb 4096

page_table_l2:
	resb 4096

page_table_l1:
	resb 4096

stack_bottom:
	resb 4096 * 4

stack_top:


section .rodata

; Access bits
PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0

; Flags bits
GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5

gdt64:

.NULL: equ $ - gdt64
	dq 0

.Code: equ $ - gdt64
	dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
	db 0                                        ; Base (mid, bits 16-23)
	db PRESENT | NOT_SYS | EXEC | RW            ; Access
	db GRAN_4K | LONG_MODE | 0xF                ; Flags & Limit (high, bits 16-19)
	db 0                                        ; Base (high, bits 24-31)

	;dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)

.Data: equ $ - gdt64
	dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
	db 0                                        ; Base (mid, bits 16-23)
	db PRESENT | NOT_SYS | RW                   ; Access
	db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
	db 0                                        ; Base (high, bits 24-31)

.TSS: equ $ - gdt64
	dd 0x00000068
	dd 0x00CF8900

.pointer:
	dw $ - gdt64 -1 ; length
	dq gdt64 ; address


;global long_mode_start
extern kernel_main

section .text
bits 64

long_mode_start:
	cli
	
.reloadSegments
	push gdt64.Code
	lea rax, [rel .reload_CS]
	push rax
	retfq

.reload_CS
	; load data into all data segment registers
	mov ax, gdt64.Data
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call kernel_main

	hlt
