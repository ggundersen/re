#include <stdio.h>
#include <stdlib.h>


int main()
{
    int *x = malloc(10 * sizeof(int));
    int i = 0;
    x[i++] = 3;
    x[i] = 4;
    printf("%d\n", x[0]);
    printf("%d\n", x[1]);
    return 0;
}
