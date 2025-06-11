section .note.GNU-stack noalloc noexec nowrite progbits

section .text

global check_apic_asm
global set_apic_base_asm
global get_apic_base_asm
global enable_apic_asm

check_apic_asm:
	mov eax, 1
	cpuid
	test edx, 1 << 9
	jz .retzero
	mov eax, 1
	ret
.retzero:
	mov eax, 0
	ret

set_apic_base_asm:
	pop rbx
	mov ecx, ebx
	shr rbx, 32
	and ebx, 0x0f
	shl rbx, 32
	mov ebx, ecx
	and rbx, 0xfffff0000
	mov eax, ebx
	shr rbx, 32
	mov edx, ebx
	or eax, 0x800 ; 0x800 is IA32_APIC_BASE_MSR_ENABLE
	wrmsr
	ret

get_apic_base_asm:
	rdmsr
	and eax, 0xfffff000
	mov ebx, eax
	and edx, 0x0f
	mov eax, edx
	shl rax, 32
	mov eax, ebx
	ret

enable_apic_asm:
	rdmsr
	shl rax, 32
	mov eax, edx
	shr rax, 32
	add rax, 0xf0
	mov rax, (1 << 8)
	shl rax, 32
	mov edx, eax
	shr rax, 32
	wrmsr
