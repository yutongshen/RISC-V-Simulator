void merge(short *list, int a, int b, int b_end) {
    int start = a;
    int a_end = b;
    int i = 0;
    short buff[b_end - a];
    asm("nop");
    while (a != a_end && b != b_end) {
        if (list[a] <= list[b])
            buff[i++] = list[a++];
        else 
            buff[i++] = list[b++];
    }
    while (a != a_end) buff[i++] = list[a++];
    while (b != b_end) buff[i++] = list[b++];
    while (i--) list[start + i] = buff[i];
}

void merge_sort(short *list, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = (left + right) >> 1;
    merge_sort(list, left, mid);
    merge_sort(list, mid + 1, right);
    merge(list, left, mid + 1, right + 1);
}

int main(void) {
    extern int array_size;
    extern short array_addr[];
    extern short _test_start[];

    merge_sort(array_addr, 0, array_size - 1);
    for (int i = 0; i < array_size; ++i)
        _test_start[i] = array_addr[i];
    return 0;
}
