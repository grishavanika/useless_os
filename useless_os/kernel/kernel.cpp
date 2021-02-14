#include <kernel/vga_text.h>

#if defined(__linux__) || !defined(__i386__)
#  error "i686-elf-g++ was not set as CXX compiler."
#endif

extern "C"
void kernel_main()
{
    vga::terminal_initialize();
    vga::terminal_write_string("Hello, kernel!");
}
