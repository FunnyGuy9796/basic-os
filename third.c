void main() {
    char *video_memory = (char *)0xb8000;
    const char *message = "Third stage loaded and running!";
    unsigned int i = 0;

    // Display message in video memory
    while (message[i] != '\0') {
        video_memory[i * 2] = message[i];     // Character
        video_memory[i * 2 + 1] = 0x07;      // Attribute byte (white on black)
        i++;
    }

    // Halt CPU
    while (1) {
        __asm__("hlt");
    }
}