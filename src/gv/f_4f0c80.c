/* 0x4f0c80, graveyard order type 5 (p: +8 the object id, +0xc the list, +0x10 the entry, +0x13 a delta): adds the delta to the byte +0xc of the entry, clamped to 0 .. 255; 0 or a negative reason (as 0x4f0240) */

int unk_Unk4f0c80(unsigned char *p)
{
    unsigned char *o;
    int l, v;
    char *e;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerChar)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    l = *(int *)(p + 0xc);
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), l) == 0)
        return -4;
    e = (char *)unk_Unk4ef570(*(char **)(o + 0x65), l, p[0x10]);
    if (e == 0)
        return -7;
    v = (unsigned char)e[0xc] + *(int *)(p + 0x13);
    if (v < 1)
        v = 0;
    if (v > 0xfe)
        v = 0xff;
    e[0xc] = (char)v;
    return 0;
}
