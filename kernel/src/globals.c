#include "globals.h"
#include <stdint.h>

volatile struct limine_framebuffer* framebuffer = 0;
volatile unsigned int stdout_cursor = 0;
volatile char* stdout = 0;