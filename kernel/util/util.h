#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;

    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));

    return value;
}

static inline void num_to_str(unsigned long long num, char *str, int base, int is_negative) {
    int i = 0;
    
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';

        return;
    }
    
    while (num != 0) {
        unsigned long long rem = num % base;

        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    
    if (is_negative)
        str[i++] = '-';
    
    str[i] = '\0';
    
    int start = 0;
    int end = i - 1;

    while (start < end) {
        char temp = str[start];

        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static inline void float_to_str(float num, char *str, int precision) {
    int integer_part = (int)num;
    float decimal_part = num - integer_part;
    int is_negative = num < 0;
    
    if (is_negative) {
        integer_part = -integer_part;
        decimal_part = -decimal_part;
    }
    
    num_to_str(integer_part, str, 10, is_negative);
    
    int i = 0;

    while (str[i] != '\0') i++;
    
    if (precision > 0) {
        str[i++] = '.';
        
        float rounding = 0.5f;

        for (int j = 0; j < precision; j++)
            rounding /= 10.0f;

        decimal_part += rounding;
        
        while (precision > 0) {
            decimal_part *= 10.0f;
            
            int digit = (int)decimal_part;

            if (digit > 9) digit = 9;

            str[i++] = '0' + digit;
            decimal_part -= digit;
            precision--;
        }
    }
    
    str[i] = '\0';
}

void *memmove(void *dest, const void *src, size_t n);

void *memcopy(void *dest, const void *src, size_t n);

void *memset(void *dest, int c, size_t n);

#endif