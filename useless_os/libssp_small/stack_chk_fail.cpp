#include <kernel/panic.h>

// https://wiki.osdev.org/Stack_Smashing_Protector
#include <stdint.h>

static_assert(sizeof(uintptr_t) == 4);
uintptr_t __stack_chk_guard = 0xe2dee396;

extern "C"
__attribute__((noreturn))
void __stack_chk_fail()
{
    panic("Stack Smashing Protector Fail");
}
