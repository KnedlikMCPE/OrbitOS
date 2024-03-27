#include "tty.h"
#include <limine.h>
#include <stdint.h>
#include <fonts/vga16.h>
#include <stdbool.h>
#include <utils/search.h>
#include <globals.h>
#include <stddef.h>

bool put_pixel(uint64_t x, uint64_t y, uint32_t color, struct limine_framebuffer* buff) {
    uint64_t index = y * (buff->pitch / 4) + x;
    if (index > buff->pitch * buff->height) {
        return false;
    }
    uint32_t* buff_addr = buff->address;
    buff_addr[index] = color;
    return true;
}

bool putc(uint64_t x, uint64_t y, char c, uint32_t color, struct limine_framebuffer* buff) {
    int64_t char_index = binary_search(font.Index, font.Chars, c);

    bool result = true;

    for (int i = 0; i < font.Height; i++) {
        for (int j = 0; j < 8; j++) {
            if ((font.Bitmap[char_index * 16 + i] & (0b10000000 >> j)) > 0) {
                result = result && put_pixel(x + j, y + i, color, buff);
            }
        }
    }

    return result;
}

bool puts(uint64_t x, uint64_t y, char *s, uint32_t color, struct limine_framebuffer *buff) {
    bool result = true;

    while (*s != '\0') {
        if (*s == '\n' || x >= buff->width - 8) {
            x = 0;
            y += 16;
            s++;
            continue;
        }
        if (y >= buff->height) {
            x = 0;
            y = 0;
        }
        result = result && putc(x, y, *s, color, buff);
        x += 8;
        s++;
    }

    return result;
}

void vprintf(char* s, va_list args) {
    if (stdout == NULL) {
        char buffer[(framebuffer->width / 8) * (framebuffer->height / 16)];
        stdout = buffer;
    }

    char c;
    while ((c = *s++) != '\0') {
        if (c == '%') {
            switch (*s++) {
                case 'd': {
                    int val = va_arg(args, int);
                    if (val < 0) {
                        stdout[stdout_cursor++] = '-';
                    }
                    int num = 0;
                    char buf[20];
                    int i = 0;
                    while (val > 0) {
                        buf[i++] = "0123456789"[val % 10];
                        val /= 10;
                        num++;
                    }
                    for (i = num; i > 0;) {
                        stdout[stdout_cursor++] = buf[--i];
                        if (stdout_cursor >= (framebuffer->width / 8) * (framebuffer->height / 16)) {
                            stdout_cursor = 0;
                        }
                    }
                    break;
                }
                case 'p': {
                    unsigned int val = va_arg(args, unsigned int);
                    stdout[stdout_cursor++] = '0';
                    stdout[stdout_cursor++] = 'x';
                    int num = 0;
                    char buf[16];
                    int i = 0;
                    while (val > 0) {
                        buf[i++] = "0123456789ABCDEF"[val % 16];
                        val /= 16;
                        num++;
                    }
                    for (i = num; i > 0;) {
                        stdout[stdout_cursor++] = buf[--i];
                        if (stdout_cursor >= (framebuffer->width / 8) * (framebuffer->height / 16)) {
                            stdout_cursor = 0;
                        }
                    }
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    for (int i = 0; str[i] != '\0'; ++i) {
                        stdout[stdout_cursor++] = str[i];
                        if (stdout_cursor >= (framebuffer->width / 8) * (framebuffer->height / 16)) {
                            stdout_cursor = 0;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        } else {
            stdout[stdout_cursor++] = c;
            if (stdout_cursor >= (framebuffer->width / 8) * (framebuffer->height / 16)) {
                stdout_cursor = 0;
            }
        }
    }

    for (int i = 0; i < framebuffer->pitch / 4 * framebuffer->height; i++) {
        uint32_t *ptr = framebuffer->address + i;
        *ptr = 0;
    }

    stdout[stdout_cursor] = '\0';

    puts(0, 0, stdout, 0xFFFFFF, framebuffer);
}

void kprintf(char* s, ...) {
    va_list args;
    va_start(args, s);
    vprintf(s, args);
    va_end(args);
}
