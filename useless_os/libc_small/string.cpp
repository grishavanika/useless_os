#include <string.h>

void* memcpy(void* __restrict__ dest, const void* __restrict__ src, size_t n)
{
    (void)dest;
    (void)src;
    (void)n;
    return nullptr;
}

void* memmove(void* dest, const void* src, size_t n)
{
    (void)dest;
    (void)src;
    (void)n;
    return nullptr;
}

void* memset(void* s, int c, size_t n)
{
    (void)s;
    (void)c;
    (void)n;
    return nullptr;
}

size_t strlen(const char* s)
{
    size_t length = 0;
    while (s[length])
    {
        ++length;
    }
    return ++length;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
    (void)s1;
    (void)s2;
    (void)n;
    return 0;
}
