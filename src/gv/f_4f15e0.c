/* 0x4f15e0, takes a random grave (0x4ef930) of the object o and its list entry (0x4ef900) and sends it with 0x4f1170 (type 0x1c); -1 when 0x4f1d50 (o, 0) is not 1 or nothing is found */

int gv_SendGrave(unsigned char *o)
{
    int *z;
    int *e;
    int k;

    if (unk_Unk4f1d50(o, 0) != 1)
        return -1;
    z = (int *)unk_Unk4ef930(o);
    if (z == 0)
        return -1;
    e = (int *)gv_LoadGraveyard(o, z);
    if (e == 0)
        return -1;
    k = gv_EntryIndex(*(char **)(o + 0x65), e);
    if (k == -1)
        return -1;
    return unk_Unk4f1170((char *)o, *e, k, *z, 0x1c);
}
