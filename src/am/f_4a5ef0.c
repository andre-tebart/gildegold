/* 0x4a5ef0: up to n candidates for an action with the flags fl, from two random walks over all 768 characters
   (no kinds 8 / 7 / 5 / 9 / 0x14; first the married ones (0x164 set, not 'R'), then the unmarried), accepted by
   0x5c33c0, of a rank (0x505f30) within 5 of p's and, when the window is narrower than 0..100, rated in
   [lo, hi]; ids into out[], returns the number */

unsigned int ai_Unk4a5ef0(AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out)
{
    unsigned int q[0x14];
    unsigned char *c;
    unsigned int cnt, i;
    int step, k, rank, win;
    char t;
    float r;

    step = ai_Unk5ed100[util_Random(0x10)];
    memset(q, 0, sizeof(q));
    memset(&q[7], 0xff, 8 * 4);
    win = 0;
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    q[5] = 2;
    q[1] = fl;
    rank = gm_Unk505f30(*(unsigned short *)p, 0);
    k = 0x300;
    cnt = 0;
    i = util_Random(0x300);
    if (n != 0) {
        do {
            if (k == 0)
                break;
            c = (unsigned char *)&ai_CharsT[i];
            t = c[2];
            if (t != 8 && t != 7 && t != 5 && t != 9 && t != 0x14 && c[0x164] != 0 && c[0x164] != 'R'
                && ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)i, q, 0) != 0
                && abs(rank - gm_Unk505f30(i, 0)) < 6
                && (!win || (r = gm_Liking(i, *(unsigned short *)p, 1), lo <= r && r <= hi))) {
                cnt++;
                out[cnt - 1] = (unsigned short)i;
            }
            k--;
            i = (int)(i + step) % 0x300;
        } while (cnt < n);
    }
    i = util_Random(0x300);
    for (k = 0x300; cnt < n && k != 0; k--) {
        c = (unsigned char *)&ai_CharsT[i];
        t = c[2];
        if (t != 8 && t != 7 && t != 5 && t != 9 && t != 0x14 && c[0x164] == 0
            && ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)i, q, 0) != 0
            && abs(rank - gm_Unk505f30(i, 0)) < 6
            && (!win || (r = gm_Liking(i, *(unsigned short *)p, 1), lo <= r && r <= hi))) {
            cnt++;
            out[cnt - 1] = (unsigned short)i;
        }
        i = (int)(step + i) % 0x300;
    }
    return cnt;
}
