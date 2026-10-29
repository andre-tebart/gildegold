/* 0x4f25d0, as 0x4f2540 with the match 0x4ef740 */

int unk_Unk4f25d0(void *a, unsigned char **o, int *e)
{
    int i, r;
    unsigned char *b;

    i = 0;
    *o = 0;
    *e = 0;
    b = (unsigned char *)ai_Blds;
    do {
        if (*b != 0 && ai_BTypes[*b * 0x24d] == 0x1d) {
            r = gv_FindEntry(a, b, 0, gv_EntrySameId, 0xff);
            if (r != 0) {
                *o = b;
                *e = r;
                return 1;
            }
        }
        i++;
        b += 0xa9;
    } while (i < 0x100);
    return 0;
}
