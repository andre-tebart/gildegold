/* 0x4f0240, the graveyard order of the message p (+4 a character, +8 an object id, +0xc the list, +0x10 the entry): the object's graveyard gets a new record of the dead character (names +0x30 / +0x40, +0x80 .., age
   and skills; time 0x770ae0); returns 0, or a negative reason (-1 no object / own object, -2 not a graveyard, -3 no record, -4 no list, -6 no character, -7 the entry does not fit). ROUGH */

int unk_Unk4f0240(int *p)
{
    unsigned char *o;
    char *c;
    int *rec, *e, *z, *best, *q;
    int *lst;
    int i, n, list;
    char *s, *d;
    char ch;

    gm_FindById(&o, 0, 0, p[2]);
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(int **)(o + 0x65);
    if (rec == 0)
        return -3;
    if (unk_Unk4ef4f0((char *)rec, p[3]) == 0)
        return -4;
    gm_FindById(0, 0, &c, p[1]);
    if (c == 0)
        return -6;
    list = p[3];
    e = (int *)unk_Unk4ef570((char *)rec, list, (unsigned char)p[4]);
    if (e == 0 || *(short *)((char *)e + 8) != *(short *)(c + 0x50))
        return -7;
    lst = (int *)e[4];
    i = 0;
    best = lst;
    q = lst;
    if (gv_EntryCounts[list] > 0) {
        do {
            if (*best == -1)
                break;
            if (*q == -1) {
                best = lst + i * 0x11;
                break;
            }
            if (q[0x10] < best[0x10])
                best = q;
            i++;
            q += 0x11;
        } while (i < gv_EntryCounts[list]);
    }
    *best = *(int *)(c + 4);
    s = c + 0x30;
    d = (char *)(best + 1);
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    s = c + 0x40;
    d = (char *)(best + 5);
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    best[9] = *(int *)(c + 0x80);
    *(char *)(best + 10) = c[0x84];
    best[0xb] = (unsigned char)c[0x167];
    best[0xc] = (unsigned char)c[0xd];
    best[0xd] = (unsigned char)c[0x164];
    best[0xe] = (unsigned char)c[9];
    best[0x10] = *(int *)ai_Time;
    best[0xf] = *(int *)(c + 0x18c);
    return 0;
}
