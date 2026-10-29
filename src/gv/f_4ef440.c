/* 0x4ef440, how many of the slots (0x4ef3c0 of them, 0x44 bytes each from the table at +0x20 of the record) are in use (first word not -1) */

int unk_Unk4ef440(unsigned char *o)
{
    int n, cnt;
    int *p;

    n = unk_Unk4ef3c0(o);
    cnt = 0;
    if (n == 0)
        return 0;
    if (n > 0) {
        p = *(int **)(*(char **)(o + 0x65) + 0x20);
        do {
            if (*p != -1)
                cnt++;
            p += 0x11;
            n--;
        } while (n != 0);
    }
    return cnt;
}
