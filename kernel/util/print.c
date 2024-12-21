#include "print.h"

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[32];

    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%') {
            ptr++;

            if (*ptr == 'l') {
                ptr++;

                if (*ptr == 'x') {
                    long value = va_arg(args, long);

                    itoa(value, buffer, 16);
                    term_writestring(buffer);

                    continue;
                }
            }

            switch (*ptr) {
                case 'd': {
                    int value = va_arg(args, int);

                    itoa(value, buffer, 10);
                    term_writestring(buffer);

                    break;
                }

                case 'x': {
                    int value = va_arg(args, int);

                    itoa(value, buffer, 16);
                    term_writestring(buffer);

                    break;
                }

                case 's': {
                    char *value = va_arg(args, char*);

                    term_writestring(value);

                    break;
                }

                case 'c': {
                    char value = (char)va_arg(args, int);

                    term_putchar(value);

                    break;
                }

                case '%': {
                    term_putchar('%');

                    break;
                }

                default: {
                    term_putchar('%');
                    term_putchar(*ptr);

                    break;
                }
            }
        } else {
            term_putchar(*ptr);
        }
    }

    va_end(args);
}