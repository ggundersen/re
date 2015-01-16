#include <stdio.h>

int main()
{
    float n = 3.0;
    float *p = &n;
    printf("%p\n", p);
    p++;
    printf("%p\n", p);
    printf("%lu\n", sizeof(3));
    return 0;
}
