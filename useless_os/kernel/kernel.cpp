#include <kernel/vga_text.h>
#include <string.h>

#if defined(__linux__) || !defined(__i386__)
#  error "i686-elf-g++ was not set as CXX compiler."
#endif

#if (0)
static void check_ssp(const char* s)
{
    char buffer_overrun[2];
    memcpy(buffer_overrun, s, strlen(s));
    vga::terminal_write(buffer_overrun, 2);
}
#endif

extern "C"
void kernel_main()
{
    vga::terminal_initialize();
    vga::terminal_write_string("Hello, kernel!");

#if (0)
    check_ssp("long string to trigger SSP.");
#endif
}
