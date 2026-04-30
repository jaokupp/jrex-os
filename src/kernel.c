void kernel_main() {
    // 0xB8000 คือตำแหน่งหน่วยความจำที่เชื่อมกับหน้าจอ
    char* video_memory = (char*) 0xB8000;

    const char* str = "JREX-OS: BARE METAL KERNEL IS RUNNING!";
    
    // พ่นตัวอักษรลงจอ (1 ตัวอักษรใช้ 2 byte: ค่าตัวอักษร + สี)
    for(int i = 0; str[i] != '\0'; i++) {
        video_memory[i*2] = str[i];     // ตัวอักษร
        video_memory[i*2+1] = 0x0A;    // สี (0x0A = สีเขียวสว่างพื้นดำ)
    }
}
