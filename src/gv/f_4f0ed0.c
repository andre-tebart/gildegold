/* 0x4f0ed0, graveyard order type 1 (p: +0 a character id, +4 the object id, +8 the source list, +0xc the entry, +0xd the target list, +0x11 a price, +0x15 a counter): moves the grave of the entry (its 0x44-byte
   records, compacting the free ones) from the source list to the first free entry of the target list, copies the character id and short, time +10 + 0x770ae0, clears the source; with a price the owner's money changes;
   0 or a negative reason (as 0x4f0240, -7 no entry / free entry). ROUGH */

int unk_Unk4f0ed0(int *p)
{
    unsigned char *o;
    int *c;
    char *rec;
    char *src, *dst;
    int *s, *s2, *d, *send, *dend, *q;
    int n, i, list;

    gm_FindById(0, 0, &c, p[0]);
    if (c == 0)
        return -6;
    gm_FindById(&o, 0, 0, p[1]);
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    list = p[2];
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), list) == 0)
        return -4;
    src = (char *)unk_Unk4ef570(*(char **)(o + 0x65), list, *(unsigned char *)(p + 3));
    if (src != 0 && *(short *)(src + 8) == *(short *)((char *)c + 0x50)) {
        s = *(int **)(src + 0x10);
        n = gv_EntryCounts[list];
        dst = (char *)gv_FindEntry(c, o, 0, gv_EntryIsFree, *(int *)((char *)p + 0xd));
        if (dst != 0) {
            d = *(int **)(dst + 0x10);
            dend = d + gv_EntryCounts[*(int *)((char *)p + 0xd)] * 0x11;
            send = s + n * 0x11;
            for (; d < dend; d += 0x11) {
                if (*d == -1) {
                    while (1) {
                        if (send <= s)
                            goto done;
                        if (*s != -1)
                            break;
                        s += 0x11;
                    }
                    s2 = s;
                    q = d;
                    for (i = 0x11; i != 0; i--)
                        *q++ = *s2++;
                    s2 = s;
                    for (i = 0x11; i != 0; i--)
                        *s2++ = 0;
                    *s = -1;
                    if (send <= s)
                        break;
                }
            }
        done:
            *(int *)(dst + 4) = *(int *)(src + 4);
            *(int *)(src + 4) = -1;
            *(short *)(dst + 8) = *(short *)(src + 8);
            *(short *)(src + 8) = (short)0xffff;
            *(short *)(dst + 10) = (short)*(int *)ai_Time + 10;
            dst[0xc] = (char)0xff;
            *(short *)(src + 10) = 0xff;
            if (ai_Unk1460ae0 != -1)
                ai_NextObjectId = *(int *)((char *)p + 0x15);
            if (*(int *)((char *)p + 0x11) > 0) {
                gm_AddNegative(c[1], *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8), *(int *)((char *)p + 0x11));
                gm_AddObjekt(*(int *)(ai_Chars[*(unsigned short *)(o + 0x27)] + 4), *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8), 0);
            }
            return 0;
        }
    }
    return -7;
}
