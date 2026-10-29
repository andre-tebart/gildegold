/* 0x503530, the permission bits of the character p1 in the building b (or, without a building, in the object o / the character c): a bit set of what he may do there (0x1 enter, 0x4 .. 0x30 use, 0x40 .., 0x800 .., 0x2000 .., 0x4000, 0x5000 ..: the exact meaning of each bit is open) by the building type: a case per type, owner (+0x27) and place (+0x25) words compared with p1, the job list (six bytes at +0x223) searched for the job number of the character (0x5044b0). With the flag 0x6f2448 everything is allowed (-1, -2 for the types 3 / 0x21). ROUGH */

int ai_Unk587b70();                     /* 0x587b70 */
extern int ai_Unk6f2448;                /* 0x6f2448 */
int gm_BuildingJob();                     /* 0x5048e0 */
int gm_TitleGroup();                     /* 0x504ad0 */

unsigned unk_Unk503530(unsigned p1, unsigned char *b, unsigned short *o, int c)
{
    char *p;
    unsigned v, jn, w;
    char t, cc;
    int r, jb;
    unsigned char *q;
    unsigned short ob;

    v = 0;
    p = 0;
    if (b == 0) {
        if (c == 0) {
            if (o == 0)
                goto fin;
            ob = *o;
            if (ai_Goods[ob * 0x41] == 6) {
                if (ob < 0x91 || ob > 0x97)
                    v = 0xffffffff;
                else
                    v = 0x4001;
            } else if (ob < 0x91 || ob > 0x97)
                v = (-(unsigned)(ob != 0xfd) & 0xffffbffe) + 0x4001;
            else
                v = 1;
        } else if (*(int *)(c + 0x16c) != 0 && *(unsigned short *)(*(int *)(c + 0x16c) + 0x27) == p1)
            v = 0x800;
        goto dflt;
    }
    p = ai_BTypes + (unsigned)*b * 0x24d;
    jb = gm_BuildingJob(*p);
    jn = gm_Unk5044b0(p1, jb);
    t = *p;
    if (t == 0x1d)
        return ai_Unk587b70(ai_Chars[p1], b, o);
    switch (t) {
    case 1:
    case 3:
    case 0xf:
        if (t != 3)
            v = 0x5081;
        cc = ai_Chars[p1][0x166];
        if (cc == 0xf || cc == 0xe || cc == 0xd)
            v |= 0x502;
        if (cc == 0xf || cc == 0xe || cc == 0xd)
            v |= 0x200;
        if (t == 3)
            r = cc == 10;
        else
            r = cc == 0xf;
        if (r)
            v |= 0x30;
        if (o == 0)
            goto fin;
        if (*o == 0x114 && cc == 0)
            v &= 0xffffbffe;
    case 0x1b:
    case 0x1d:
        break;
    case 2:
    case 0x1e:
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x3c;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x5661;
        break;
    case 4:
    case 0x13:
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x50bd;
        if (*(unsigned short *)(b + 0x27) == p1) {
            v |= 0x44e1;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v |= 0x2200;
                    goto dflt;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        }
        break;
    case 5:
    case 8:
    case 0xe:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x20:
        v = 0x81;
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x10bd;
        if (*(unsigned short *)(b + 0x27) == p1) {
            w = v | 0x460;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0) {
                set:
                    v = w | 0x4000;
                    goto dflt;
                }
                if (jn == *q) {
                    w = v | 0x2660;
                    goto set;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
            v |= 0x4460;
        }
        break;
    case 6:
        v = 0x5001;
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x503d;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x5660;
        if (ai_Chars[p1][0x166] == 0xf)
            v |= 0x30;
        break;
    case 7:
        v = 1;
        if (o != 0 && (*o == 0xe6 || *o == 0xe5))
            v = 0x1001;
        if (*(unsigned short *)(b + 0x25) == p1)
            v |= 0x103c;
        if (*(unsigned short *)(b + 0x27) == p1) {
            v |= 0x4460;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v |= 0x2200;
                    goto dflt;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        } else {
            if (o == 0)
                goto fin;
            if (*o != 0xf3)
                v |= 0x5000;
        }
        break;
    case 9:
        v = 1;
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x403d;
        if (*(unsigned short *)(b + 0x27) == p1) {
            v |= 0x54e1;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v |= 0x2200;
                    goto dflt;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        }
        break;
    case 10:
        v = 1;
        break;
    case 0xb:
    case 0xc:
    case 0xd:
    case 0x11:
    case 0x1f:
        if (o == 0 || *(unsigned short *)(b + 0x27) == p1)
            v = 0x81;
        if (*(unsigned short *)(b + 0x25) == p1)
            v |= 0x403c;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x5670;
        if (o == 0)
            goto fin;
        if (*o == 0xfd)
            v |= 0x4001;
        break;
    case 0x10:
        if (*(unsigned short *)(b + 0x27) == p1) {
            v = 0x54e1;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v = 0x76e1;
                    break;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        }
        if (*(unsigned short *)(b + 0x25) == p1)
            v |= 0x3c;
        break;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        v = 0x5081;
        if ((t == 0x17 && ai_Chars[p1][9] == 0x1e) || (t == 0x18 && ai_Chars[p1][9] == 0x1f) ||
            (t == 0x19 && ai_Chars[p1][9] == 0x20) || (t == 0x1a && ai_Chars[p1][9] == 0x21))
            v = 0x52f1;
        break;
    case 0x1c:
        r = gm_TitleGroup(*(int *)(ai_Chars[p1] + 0x164));
        if (r == 8 || gm_BuildQuery(2, 5, 9, 4, *(short *)ai_Chars[p1]) != 0)
            v = 1;
        break;
    case 0x21:
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x3c;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x620;
    }
dflt:
    if (o != 0) {
        ob = *o;
        if (ob == 0x27f || ob == 0x21b || ob == 0x28a || ob == 0x120)
            v |= 0x5000;
        if (ob == 0x12e)
            v |= 0x4000;
    }
fin:
    if (ai_Unk6f2448 != 0) {
        v = 0xffffffff;
        if (b == 0)
            return 0xffffffff;
        if (*p == 3 || *p == 0x21)
            v = 0xfffffffe;
    }
    if (b != 0 && *(short *)(b + 0x27) == -1)
        v = 0;
    return v;
}
