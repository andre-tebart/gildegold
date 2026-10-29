/* 0x4eedd0, the effect slot of the id or 0 */

char *unk_Unk4eedd0(int id)
{
    int *p;
    int i;

    i = 0;
    p = (int *)gv_Effects;
    do {
        if (*p == id)
            return gv_Effects + i * 0x170;
        p += 0x5c;
        i++;
    } while ((int)p < (int)((unsigned)&gv_Effects + 0xb80));
    return 0;
}
