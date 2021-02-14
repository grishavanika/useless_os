#include <stdio.h>

int printf(const char* __restrict__ format, ...)
{
    (void)format;
    return 0;
}

int putchar(int c)
{
    (void)c;
    return 0;
}

int puts(const char* s)
{
    (void)s;
    return 0;
}
