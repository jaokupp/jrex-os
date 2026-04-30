all:
	# 1. แปลง Assembly โดยระบุ target เป็น i386 (x86 32-bit)
	clang -target i386-pc-none-elf -c src/boot.s -o boot.o

	# 2. คอมไพล์ C โดยระบุ target เดียวกัน
	clang -target i386-pc-none-elf -ffreestanding -c src/kernel.c -o kernel.o

	# 3. เชื่อมไฟล์ (Link)
	ld.lld -m elf_i386 -T linker.ld boot.o kernel.o -o jrex_os.bin

clean:
	rm -f *.o jrex_os.bin
