#include <stdio.h>

#if 0
__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns")))
void *memset(void *s, int c, size_t n)
{
    if (s) {
        unsigned char *p = s, *p_end = p + n;
        unsigned char v = (unsigned char)c;
        while (p < p_end)
            *p++ = v;
    }
    return s;
}

void *memmove(void *dest, const void *src, size_t n)
{
    char *p_dst = (char*)dest, *p_dst_end = p_dst + n;
    char *p_src = (char*)src;

    while (p_dst < p_dst_end)
        *p_dst++ = *p_src++;
    return dest;
}

void *memcpy(void *dest, void *src, size_t n) 
{ 
    char *csrc = (char *)src; 
    char *cdest = (char *)dest; 

    for (size_t i = 0; i < n; i++) 
        cdest[i] = csrc[i]; 
    return dest;
} 
       
size_t
strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s)
        ;
    return (s - str);
}

#endif
