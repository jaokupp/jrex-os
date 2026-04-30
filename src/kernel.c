/* --- 1. ฟังก์ชันช่วย (ต้องอยู่บนสุด) --- */

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void delay(int count) {
    volatile int i;
    for (i = 0; i < count * 10000000; i++) {} 
}

void clear_screen() {
    char* video_memory = (char*) 0xB8000;
    for(int i = 0; i < 80 * 25 * 2; i++) video_memory[i] = 0;
}

void print(const char* str, int row, int col, char color) {
    char* video_memory = (char*) 0xB8000;
    int index = (row * 80 + col) * 2;
    for(int i = 0; str[i] != '\0'; i++) {
        video_memory[index + (i * 2)] = str[i];
        video_memory[index + (i * 2) + 1] = color;
    }
}

// ตารางคีย์บอร์ด
unsigned char scancode_to_char[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

/* --- 2. ฟังก์ชันหลัก (ต้องอยู่ล่างสุด) --- */

void kernel_main() {
    clear_screen();
    print("JREX-OS IS LOADING...", 10, 30, 0x0E);
    delay(20); 
    
    clear_screen();
    print("JREX-OS TERMINAL READY.", 0, 0, 0x0F);
    
    int cursor = 160; 
    while(1) {
        if(inb(0x64) & 0x01) {
            unsigned char scancode = inb(0x60);
            if (!(scancode & 0x80) && scancode < 58) {
                char c = scancode_to_char[scancode];
                if (c > 0) {
                    char str[2] = {c, '\0'};
                    print(str, (cursor/160), (cursor%160)/2, 0x0F);
                    cursor += 2;
                }
            }
        }
    }
}
