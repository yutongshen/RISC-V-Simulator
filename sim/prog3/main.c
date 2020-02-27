void div(int div1, int div2, int *target) {
    unsigned long long int *res = (unsigned long long int *) target;
    char flag1 = div1 < 0;
    char flag2 = div2 < 0;
    if (flag1) div1 = -div1;
    if (flag2) div2 = -div2;
    target[0] = div1;
    int i = sizeof(div1) << 3;
    while (i--) {
        *res <<= 1;
        if (target[1] >= div2)
            target[1] -= div2, target[0] |= 1;
    }
    if (flag1 ^ flag2) target[0] = -target[0];
    if (flag1) target[1] = -target[1];
}

int main(void) {
    extern int div1;
    extern int div2;
    extern int _test_start[2];
    div(div1, div2, _test_start);
    return 0;
}
