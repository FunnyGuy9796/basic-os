#include "print.h"

void kprint(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    int chars_written = 0;
    
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            term_putchar(format[i]);

            chars_written++;

            continue;
        }
        
        i++;
        
        int length_modifier = 0;

        if (format[i] == 'l') {
            length_modifier = 1;
            i++;
            
            if (format[i] == 'l') {
                length_modifier = 2;
                i++;
            }
        }

        int precision = 6;
        char* peek = (char*)&format[i];

        if (*peek == '.') {
            peek++;
            precision = 0;

            while (*peek >= '0' && *peek <= '9') {
                precision = precision * 10 + (*peek - '0');
                peek++;
            }

            i += (peek - &format[i]) - 1;

            if (format[i + 1] == 'f') i++;
        }
        
        switch (format[i]) {
            case 'd': {
                long long num;
                int is_negative = 0;
                
                if (length_modifier == 0)
                    num = va_arg(args, int);
                else if (length_modifier == 1)
                    num = va_arg(args, long);
                else
                    num = va_arg(args, long long);
                
                if (num < 0) {
                    is_negative = 1;
                    num = -num;
                }
                
                num_to_str(num, buffer, 10, is_negative);
                
                for (int j = 0; buffer[j] != '\0'; j++) {
                    term_putchar(buffer[j]);

                    chars_written++;
                }

                break;
            }

            case 'u': {
                unsigned long long num;

                if (length_modifier == 0)
                    num = va_arg(args, unsigned int);
                else if (length_modifier == 1)
                    num = va_arg(args, unsigned long);
                else
                    num = va_arg(args, unsigned long long);
                
                num_to_str(num, buffer, 10, 0);

                for (int j = 0; buffer[j] != '\0'; j++) {
                    term_putchar(buffer[j]);

                    chars_written++;
                }

                break;
            }

            case 'x': {
                unsigned long long num;
                
                if (length_modifier == 0)
                    num = va_arg(args, unsigned int);
                else if (length_modifier == 1)
                    num = va_arg(args, unsigned long);
                else
                    num = va_arg(args, unsigned long long);
                
                num_to_str(num, buffer, 16, 0);
                
                for (int j = 0; buffer[j] != '\0'; j++) {
                    term_putchar(buffer[j]);

                    chars_written++;
                }

                break;
            }

            case 'f': {
                double num = va_arg(args, double);

                float_to_str((float)num, buffer, precision);
                
                for (int j = 0; buffer[j] != '\0'; j++) {
                    term_putchar(buffer[j]);

                    chars_written++;
                }

                break;
            }

            case 's': {
                char *str = va_arg(args, char*);
                
                for (int j = 0; str[j] != '\0'; j++) {
                    term_putchar(str[j]);

                    chars_written++;
                }

                break;
            }

            case 'p': {
                void *ptr = va_arg(args, void*);

                num_to_str((unsigned long long)ptr, buffer, 16, 0);

                for (int j = 0; buffer[j] != '\0'; j++) {
                    term_putchar(buffer[j]);

                    chars_written++;
                }

                break;
            }

            case '%': {
                term_putchar('%');

                chars_written++;

                break;
            }
        }
    }

    va_end(args);
}