void mul(int mul1, int mul2, int *target)
{
    unsigned long long int *res = (unsigned long long int *) target;
    char flag1 = mul1 < 0;
    char flag2 = mul2 < 0;
    if (flag1)
        mul1 = -mul1;
    if (flag2)
        mul2 = -mul2;
    *res = 0UL;
    int i = sizeof(mul1) << 3;
    while (i--) {
        if (mul1 & 1)
            target[1] += mul2;
        *res >>= 1;
        mul1 >>= 1;
    }
    if (flag1 ^ flag2)
        *res = -*res;
}

int main(void)
{
    extern int mul1;
    extern int mul2;
    extern int _test_start[2];
    mul(mul1, mul2, _test_start);
    return 0;
}
