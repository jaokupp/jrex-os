/* Helpers */
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

void draw_progress_bar(int row, int col, int width, int progress, char color) {
    print("[", row, col, 0x07);
    for (int i = 0; i < width; i++) {
        if (i < progress) print("=", row, col + 1 + i, color);
        else print(".", row, col + 1 + i, 0x08);
    }
    print("]", row, col + 1 + width, 0x07);
}

unsigned char scancode_to_char[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

/* Main Kernel */
void kernel_main() {
    clear_screen();
    char logo_color = 0x0B;

    // Logo ASCII ที่พี่ต้องการ
    print("    /$$$$$                                       /$$$$$$   /$$$$$$ ", 3, 5, logo_color);
    print("   |__  $$                                      /$$__  $$ /$$__  $$", 4, 5, logo_color);
    print("      | $$  /$$$$$$   /$$$$$$  /$$   /$$       | $$  \\ $$| $$  \\__/", 5, 5, logo_color);
    print("      | $$ /$$__  $$ /$$__  $$|  $$ /$$//$$$$$$| $$  | $$|  $$$$$$ ", 6, 5, logo_color);
    print(" /$$  | $$| $$  \\__/| $$$$$$$$ \\  $$$$/|______/| $$  | $$ \\____  $$", 7, 5, logo_color);
    print("| $$  | $$| $$      | $$_____/  >$$  $$        | $$  | $$ /$$  \\ $$", 8, 5, logo_color);
    print("|  $$$$$$/| $$      |  $$$$$$$ /$$/\\  $$       |  $$$$$$/|  $$$$$$/", 9, 5, logo_color);
    print(" \\______/ |__/       \\_______/|__/  \\__/        \\______/  \\______/ ", 10, 5, logo_color);

    for (int p = 0; p <= 20; p++) {
        draw_progress_bar(15, 30, 20, p, 0x0A);
        delay(10);
    }

    delay(20);
    clear_screen();
    print("JREX-OS TERMINAL v1.0", 0, 0, 0x0F);
    print("Ready for input...", 1, 0, 0x07);

    int cursor = 160 * 2;
    char* video_memory = (char*) 0xB8000;

    while(1) {
        if(inb(0x64) & 0x01) {
            unsigned char scancode = inb(0x60);
            if (!(scancode & 0x80)) {
                if (scancode == 0x1C) cursor = ((cursor / 160) + 1) * 160;
                else if (scancode < 58) {
                    char c = scancode_to_char[scancode];
                    if (c > 0) {
                        video_memory[cursor] = c;
                        video_memory[cursor+1] = 0x0F;
                        cursor += 2;
                    }
                }
            }
        }
    }
}
