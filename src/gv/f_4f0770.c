/* 0x4f0770, graveyard order type 2 (p: +4 a character id, +8 the object id, +0xc the list, +0x10 the entry, +0x11 a time to add, +0x13 a price): adds the time to the entry's short +10; with a price the
   owner's money changes (0x500ae0 / 0x500970); 0 or a negative reason (as 0x4f0240). ROUGH */

int unk_Unk4f0770(char *p)
{
    unsigned char *o;
    int *c;
    char *rec;
    short *e;
    int ow;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(char **)(o + 0x65);
    if (rec == 0)
        return -3;
    if (unk_Unk4ef4f0(rec, *(int *)(p + 0xc)) == 0)
        return -4;
    gm_FindById(0, 0, &c, *(int *)(p + 4));
    if (c == 0)
        return -6;
    e = (short *)unk_Unk4ef570(rec, *(int *)(p + 0xc), (unsigned char)p[0x10]);
    if (e == 0)
        return -7;
    e[5] = e[5] + *(short *)(p + 0x11);
    if (*(int *)(p + 0x13) > 0) {
        ow = *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8);
        gm_AddNegative(c[1], ow, *(int *)(p + 0x13));
        gm_AddObjekt(*(int *)(ai_Chars[*(unsigned short *)(o + 0x27)] + 4), ow, 0);
    }
    return 0;
}
