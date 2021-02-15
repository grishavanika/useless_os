#include <kernel/panic.h>
#include <kernel/vga_text.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void panic(const char* msg)
{
    vga::terminal_write_string(msg);
    vga::terminal_write_string("\n");
    abort();
}

