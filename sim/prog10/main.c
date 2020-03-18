#include <stdio.h>
#include "util.h"

int main(int argc, char **argv)
{
    int a = 0;
    int b = 1;
    a = a + b;
    printf("hello world!!!\n");
    printf("a = %d, b = %d\n", a, ++b);
    return a;
}
