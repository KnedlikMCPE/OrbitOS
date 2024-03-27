#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
struct limine_framebuffer;
bool put_pixel(uint64_t x, uint64_t y, uint32_t color, struct limine_framebuffer* buff);
bool putc(uint64_t x, uint64_t y, char c, uint32_t color, struct limine_framebuffer* buff);
bool puts(uint64_t x, uint64_t y, char* s, uint32_t color, struct limine_framebuffer* buff);
void vprintf(char* s, va_list args);
void kprintf(char* s, ...);

#endif // TTY_H
