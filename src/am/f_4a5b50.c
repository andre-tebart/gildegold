/* 0x4a5b50: as 0x4a58d0, but the partners come from a random walk over all 768 characters (steps 0x5ed100);
   the rating window [lo, hi] (0x50c550) only counts when it is narrower than 0..100 */

int ai_Unk4a5b50(AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int i, c;
    int step, n, found, win;
    float r;

    step = ai_Unk5ed100[util_Random(0x10)];
    i = 0;
    memset(q, 0, sizeof(q));
    win = 0;
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[16] = 0;
        q[1] = flags[0];
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            i = 1;
            id[0] = *(unsigned short *)ai_Partner;
        }
    }
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    for (; i < a; i++) {
        q[1] = flags[i];
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
                    return 0;
                break;
            }
            if (ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)c, q, 0) != 0) {
                if (!win || (r = gm_Liking(c, *(unsigned short *)p, 1), lo <= r && r <= hi)) {
                    found = 1;
                    id[i] = (unsigned short)c;
                }
            }
            n--;
            c = (int)(c + step) % 0x300;
        }
    }
    return 1;
}
