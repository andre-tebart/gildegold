/* 0x4f1200, raises the dead character (kind 0x14) of the graveyard entry p[+0x11] (list +0xc, entry +0x10 of the object +8) as a living one: the first free character slot (as 0x4ec500), skills = record bytes
   +0x24 >> 1, names from the record (+4 / +0x14), a random float 1000 / (rand * 4 + 8), relations random -0x20 .. 0x1f; the entry is cleared, and the first other used entry becomes the list's current one; returns
   the character id or 0xffff / a negative reason (as 0x4f0240). ROUGH */

unsigned unk_Unk4f1200(char *p)
{
    unsigned char *o;
    char *rec, *e, *c;
    int *z, *z2, *q;
    int l, n, i, j, k, slot;
    float r;

    if (*(int *)(p + 0x15) != 0x1b && *(int *)(p + 0x15) != 0x1c)
        return (unsigned)-6;
    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return (unsigned)-1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return (unsigned)-2;
    if (*(int *)(o + 0x65) == 0)
        return (unsigned)-3;
    l = *(int *)(p + 0xc);
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), l) == 0)
        return (unsigned)-4;
    e = (char *)unk_Unk4ef570(*(char **)(o + 0x65), l, (unsigned char)p[0x10]);
    if (e == 0)
        return 0xffff;
    i = 0;
    if (gv_EntryCounts[l] > 0) {
        z = *(int **)(e + 0x10);
        do {
            q = z;
            if (*q == *(int *)(p + 0x11))
                break;
            i++;
            z = q + 0x11;
        } while (i < gv_EntryCounts[l]);
        if (*q == *(int *)(p + 0x11)) {
            for (slot = 0; slot < 0x300; slot++)
                if (*(short *)ai_Chars[slot] == -1)
                    break;
            if (slot == 0x300) {
                for (slot = 0; slot < 0x300; slot++)
                    if (ai_Chars[slot][2] == 0x0f)
                        break;
                if (slot == 0x300)
                    return 0xffff;
            }
            if (ai_Chars[slot][2] == 0x0f)
                ai_CharCount--;
            c = ai_Chars[slot];
            memset(c, 0, 0x21c);
            *(int *)(c + 4) = -1;
            *(unsigned short *)c = 0xffff;
            memset(c + 0x5c, 0xff, 0x20);
            ai_CharCount++;
            *(int *)(c + 4) = ai_NextObjectId;
            ai_NextObjectId++;
            *(unsigned short *)c = (unsigned short)slot;
            c[2] = 0x14;
            *(unsigned char **)(c + 0x170) = o;
            *(unsigned char **)(c + 0x16c) = o;
            *(int *)(c + 0x1f0) = 0;
            *(int *)(c + 0x210) = -1;
            c[0x215] = 0;
            c[0x216] = 0;
            c[0x217] = 0;
            c[0x218] = 1;
            *(float *)(c + 0x1e4) = 1.0f;
            c[8] = 100;
            *(short *)(c + 0xa) = 0x10;
            *(short *)(c + 0x164) = 0;
            c[0x165] = p[0x15];
            c[0x166] = 0;
            *(int *)(c + 0x18c) = -1;
            *(int *)(c + 0x20c) = -1;
            c[0x1f4] = 0;
            *(int *)(c + 0x190) = 4;
            *(int *)(c + 0x194) = 0;
            c[0xd] = 1;
            *(short *)(c + 0x50) = 0x7fff;
            for (j = 0; j < 5; j++)
                c[0x80 + j] = (char)((unsigned char)((char *)q)[0x24 + j] >> 1);
            c[9] = (char)q[0xe];
            {
                char *s = (char *)(q + 1);
                char *d = c + 0x30;
                char ch;
                do {
                    ch = *s++;
                    *d++ = ch;
                } while (ch != 0);
                s = (char *)(q + 5);
                d = c + 0x40;
                do {
                    ch = *s++;
                    *d++ = ch;
                } while (ch != 0);
            }
            *(int *)(c + 0x10) = 0;
            r = util_RandomF();
            *(int *)(c + 0x18) = 0;
            *(float *)(c + 0x1c) = 1000.0f;
            *(int *)(c + 0x20) = 0;
            *(int *)(c + 0x24) = 0;
            *(short *)(c + 0x28) = 0;
            *(int *)(c + 0x2c) = 0;
            k = 0;
            *(float *)(c + 0x14) = 1000.0f / (r * 4.0f + 8.0f);
            for (i = 0, j = 0; i < 0x90000; i += 0x300, j++) {
                ai_RelationMemory[0][*(unsigned short *)c + i] = 0;
                ai_Relations[0][*(unsigned short *)c + i] = (signed char)util_Random(0x40) - 0x20;
                ai_Relations[0][*(unsigned short *)c * 0x300 + j] = (signed char)util_Random(0x40) - 0x20;
                ai_RelationMemory[0][*(unsigned short *)c * 0x300 + j] = 0;
            }
            gm_PlaceOnStage(*(int *)(c + 4), *(int *)(o + 1), -1, 0, 0);
            gm_ChooseHeadIcon(c);
            rec = *(char **)(o + 0x65);
            if (*q == *(int *)(e + 4)) {
                n = 0;
                if (gv_EntryCounts[*(int *)(p + 0xc)] > 0) {
                    z2 = *(int **)(e + 0x10);
                    do {
                        if (*z2 != -1 && *z2 != *q) {
                            *(int *)(e + 4) = *z2;
                            goto cleared;
                        }
                        n++;
                        z2 += 0x11;
                    } while (n < gv_EntryCounts[*(int *)(p + 0xc)]);
                }
                *(int *)(e + 4) = -1;
                *(short *)(e + 8) = (short)0xffff;
            }
        cleared:
            z2 = q;
            for (n = 0x11; n != 0; n--)
                *z2++ = 0;
            *q = -1;
            return *(unsigned short *)c;
        }
    }
    return 0xffff;
}
