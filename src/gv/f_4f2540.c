/* 0x4f2540, finds the first graveyard (building kind 0x1d) of the 256 buildings whose list 0xff (0x4ef7c0) has an entry matching `a` by 0x4ef710; *o = the building, *e = the entry; 1 when found */

int unk_Unk4f2540(void *a, unsigned char **o, int *e)
{
    int i, r;
    unsigned char *b;

    i = 0;
    *o = 0;
    *e = 0;
    b = (unsigned char *)ai_Blds;
    do {
        if (*b != 0 && ai_BTypes[*b * 0x24d] == 0x1d) {
            r = gv_FindEntry(a, b, 0, unk_Unk4ef710, 0xff);
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
