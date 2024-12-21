#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;

    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));

    return value;
}

static inline void itoa(int value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (value == 0) {
        *str++ = '0';
        *str = '\0';
        return;
    }

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    tmp_value = value;

    while (tmp_value) {
        int rem = tmp_value % base;
        *ptr++ = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        tmp_value /= base;
    }

    *ptr = '\0';

    while (ptr1 < --ptr) {
        tmp_char = *ptr1;
        *ptr1++ = *ptr;
        *ptr = tmp_char;
    }
}

#endif