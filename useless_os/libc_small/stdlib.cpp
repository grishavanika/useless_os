#include <stdlib.h>

__attribute__((__noreturn__))
void abort()
{
    while (true) { }
    __builtin_unreachable();
}
