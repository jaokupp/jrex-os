# ประกาศค่ามาตรฐาน Multiboot
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # จอง RAM 16KB สำหรับ Stack
stack_top:
# ... (ส่วนบนเหมือนเดิม) ...

.section .text
.global _start
_start:
	mov $stack_top, %esp   # ตั้งค่า Stack pointer
	
	# ลบคำว่า extern ออก แล้วเรียกได้เลย
	call kernel_main       
	
	cli
1:	hlt
	jmp 1b
