#include "tty.h"
#include <limine.h>
#include <stdint.h>
#include <fonts/vga16.h>
#include <stdbool.h>
#include <utils/search.h>

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
