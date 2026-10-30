/* 0x4f0970, graveyard order type 3 (p: +4 a character id or -1, +8 the object id, +0xc the list, +0x10 the entry): sets the entry's id (+4) and short (+8) to the character's (-1 / 0xffff when none);
   0 or a negative reason (as 0x4f0240). ROUGH */

int unk_Unk4f0970(unsigned char *p)
{
    unsigned char *o;
    int *c;
    char *rec;
    int id;
    short sh;
    char *e;

    id = -1;
    sh = (short)0xffff;
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
    if (*(int *)(p + 4) != -1) {
        gm_FindById(0, 0, &c, *(int *)(p + 4));
        if (c == 0)
            return -6;
        id = *(int *)(p + 4);
        sh = *(short *)((char *)c + 0x50);
    }
    e = (char *)unk_Unk4ef570(rec, *(int *)(p + 0xc), p[0x10]);
    if (e == 0)
        return -7;
    *(short *)(e + 8) = sh;
    *(int *)(e + 4) = id;
    return 0;
}
