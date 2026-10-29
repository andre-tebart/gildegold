/* 0x4f2870, finds the record of the table whose bytes +0x1c / +0x1d are (c[+0x166], b) and copies it to out; 0 when none */

int gv_FindTableRecord(char *c, unsigned b, int *out)
{
    unsigned char *p;
    int i, k;
    int *s;

    i = 0;
    p = (unsigned char *)unk_Tab611258 + 0x1d;
    while ((unsigned char)c[0x166] != p[-1] || b != *p) {
        p += 0x24;
        i++;
        if ((int)p > ((unsigned)&unk_Tab611258 + 0x430))
            return 0;
    }
    s = (int *)(unk_Tab611258 + i * 0x24);
    for (k = 9; k != 0; k--)
        *out++ = *s++;
    return 1;
}
