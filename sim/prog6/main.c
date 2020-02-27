int main(void) {
    extern unsigned char _binary_image_bmp_start[];
    extern unsigned char _binary_image_bmp_end[];
    extern unsigned char _test_start[];
    unsigned char *ptr = _binary_image_bmp_start;
    unsigned char pixel;
    int i = 0;
    while (i != 54)
        _test_start[i++] = *(ptr++);
    while (ptr < _binary_image_bmp_end) {
        _test_start[i] = _test_start[i + 1] = _test_start[i + 2] = (11 * ptr[0] + 59 * ptr[1] + 30 * ptr[2]) / 100;
        ptr += 3, i += 3;
    }
    return 0;
}
