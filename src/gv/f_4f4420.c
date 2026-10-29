/* 0x4f4420, a check of the character c for the order kind k: -1 without c, -3 for k >= 10, 0 for other kinds than 5 / 7, else -4 when the byte (+0x166 for 5, +0xd for 7) is 0 */

int unk_Unk4f4420(char *c, unsigned k)
{
    int r;
    char b;

    r = 0;
    if (c == 0)
        return -1;
    if (k < 10) {
        if (k == 5)
            b = c[0x166];
        else {
            if (k != 7)
                return 0;
            b = c[0xd];
        }
        if (b == 0)
            r = -4;
        return r;
    }
    return -3;
}
