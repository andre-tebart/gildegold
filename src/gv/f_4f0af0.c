/* 0x4f0af0, graveyard order type 4 (p: +4 a character id, +8 the object id, +0xc the list 1 .. 3): switches on the list for the record rec (flags +0x19 / +0x1a / +0x1b = 1 for the lists up to it); 0 or a negative reason
   (-8 other list, -7 the list is missing or the kind too small, as 0x4f0240 otherwise) */

int unk_Unk4f0af0(char *p)
{
    unsigned char *o;
    char *rec;
    int *c;
    int l;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerChar)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(char **)(o + 0x65);
    if (rec == 0)
        return -3;
    gm_FindById(0, 0, &c, *(int *)(p + 4));
    if (c == 0)
        return -6;
    l = *(int *)(p + 0xc);
    if (l == 3) {
        if (*(int *)(rec + 0x14) != 0 && *(int *)(rec + 4) > 2) {
            rec[0x1b] = 1;
            rec[0x1a] = 1;
            rec[0x19] = 1;
            return 0;
        }
    } else if (l == 2) {
        if (*(int *)(rec + 0x10) != 0 && *(int *)(rec + 4) > 1) {
            rec[0x1a] = 1;
            rec[0x19] = 1;
            return 0;
        }
    } else {
        if (l != 1)
            return -8;
        if (*(int *)(rec + 0xc) != 0) {
            rec[0x19] = 1;
            return 0;
        }
    }
    return -7;
}
