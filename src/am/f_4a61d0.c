/* 0x4a61d0: as 0x4a5b50 with one flag set fl for every slot, and the partners' quarter (0x166, 0x4e7c40) at most
   d away from p's; returns the number found */

unsigned int ai_Unk4a61d0(AIPER *p, unsigned int a, unsigned int fl, float lo, float hi, int d, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int i, c;
    int step, n, found, win, home;
    float r;

    step = ai_Unk5ed100[util_Random(0x10)];
    i = 0;
    memset(q, 0, sizeof(q));
    win = 0;
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[1] = fl;
        q[16] = 0;
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            i = 1;
            id[0] = *(unsigned short *)ai_Partner;
        }
    }
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    home = amt_GetKindLevel(((unsigned char *)p)[0x166]);
    for (; i < a; i++) {
        q[1] = fl;
        if (i == 0)
            *(unsigned short *)&q[7] = *(unsigned short *)p;
        else
            ((unsigned short *)&q[7])[i] = id[i - 1];
        n = 0x300;
        found = 0;
        c = util_Random(0x300);
        while (!found) {
            if (n == 0) {
                if (!found)
                    return i;
                break;
            }
            if (ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)c, q, 0) != 0
                && (!win || (r = gm_Liking(c, *(unsigned short *)p, 1), lo <= r && r <= hi))
                && abs(home - amt_GetKindLevel(((unsigned char *)&ai_Chars[c])[0x166])) <= d) {
                found = 1;
                id[i] = (unsigned short)c;
            }
            n--;
            c = (int)(c + step) % 0x300;
        }
    }
    return i;
}
