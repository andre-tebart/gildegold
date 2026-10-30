/* 0x4f0490, graveyard order type 0 (p: +8 the object id, +0xc the list, +0x10 / +0x11 / +0x13 values): the first free entry (+4 = -1) of the list gets the character p[+4]'s id, its short +0x50 and the time
   +0x11 + 0x770ae0; its record is cleared (ids -1); with a price (+0x13 > 0) the owner's money changes (0x500ae0 / 0x500970) and a player character (kind 6) is told (0x5a96e0). The result 0, or a negative reason
   (as 0x4f0240; -7 no free entry, -5 list out of range). ROUGH (the floats of the __ftol calls are lost) */

int unk_Unk4f0490(short *p)
{
    unsigned char *o;
    char *c;
    int *e;
    int i, n;
    int ow;
    unsigned short ch;

    gm_FindById(&o, 0, 0, *(int *)(p + 4));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), *(int *)(p + 6)) == 0)
        return -4;
    gm_FindById(0, 0, &c, *(int *)(p + 2));
    if (c == 0)
        return -6;
    if (*(int *)(p + 6) >= 0 && *(int *)(p + 6) < 4) {
        e = (int *)gv_FindEntry(c, o, 0, gv_EntryIsFree, *(int *)(p + 6));
        if (e == 0)
            return -7;
        e[1] = *(int *)(c + 4);
        *(short *)(e + 2) = *(short *)(c + 0x50);
        *(short *)((char *)e + 10) = *(short *)((char *)p + 0x11) + (short)*(int *)ai_Time;
        *((char *)e + 0xc) = (char)0xff;
        i = 0;
        if (gv_EntryCounts[*(int *)(p + 6)] > 0) {
            n = 0;
            do {
                i++;
                *(int *)(n + e[4]) = -1;
                n += 0x44;
            } while (i < gv_EntryCounts[*(int *)(p + 6)]);
        }
        if (*(int *)((char *)p + 0x13) > 0) {
            ow = *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8);
            gm_AddNegative(*(int *)(c + 4), ow, *(int *)((char *)p + 0x13));
            gm_AddObjekt(*(int *)(ai_Chars[*(unsigned short *)(o + 0x27)] + 4), ow, 0);
            ch = *(unsigned short *)(o + 0x27);
            if (c != ai_Chars[ch] && ai_Chars[ch][2] == 6)
                ai_Unk5a96e0(ai_Chars[ch], c, o, *(int *)(p + 6), *(int *)((char *)p + 0x13), 0);
        }
        return 0;
    }
    return -5;
}
