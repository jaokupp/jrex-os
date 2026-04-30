all:
	# แปลง Assembly เป็น Object file
	as --32 src/boot.s -o boot.o
	# คอมไพล์ C แบบไม่พึ่งพา OS อื่น (-ffreestanding)
	clang -m32 -ffreestanding -c src/kernel.c -o kernel.o
	# เชื่อมไฟล์ (Link) เป็นไบนารีสำหรับบูต
	ld -m elf_i386 -T linker.ld boot.o kernel.o -o jrex_os.bin

clean:
	rm -f *.o jrex_os.bin
	
