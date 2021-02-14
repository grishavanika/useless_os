#pragma once
#include <stdint.h>
#include <stddef.h>

// https://github.com/phil-opp/blog_os/blob/post-03/src/vga_buffer.rs

namespace vga
{
    enum class Color : uint8_t
    {
        Black         = 0,
        Blue          = 1,
        Green         = 2,
        Cyan          = 3,
        Red           = 4,
        Magenta       = 5,
        Brown         = 6,
        LightGrey     = 7,
        DarkGrey      = 8,
        LightBlue     = 9,
        LightGreen    = 10,
        LightCyan     = 11,
        LightRed      = 12,
        LightMagenta  = 13,
        LightBrown    = 14,
        White         = 15,
    };

    void terminal_initialize();
    void terminal_putchar(char c);
    void terminal_write(const char* data, size_t size);
    void terminal_write_string(const char* data);
} // namespace vga
