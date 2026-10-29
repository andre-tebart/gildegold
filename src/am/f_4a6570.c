/* 0x4a6570: as 0x4a63f0, but collects up to n buildings into out[]; returns the number */

int ai_Unk4a6570(AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out)
{
    unsigned int q[0x12];
    AIBLD *b;
    int step, k, cnt, win;
    unsigned int j;
    float r;

    step = ai_Unk5ed140[util_Random(0x10)];
    cnt = 0;
    q[4] = a;
    q[3] = (unsigned int)(mask >> 32) | 0x80000000;
    win = 0;
    q[5] = 0;
    q[2] = (unsigned int)mask;
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    k = 0x100;
    j = util_Random(0x100);
    if (n != 0) {
        do {
            if (k == 0)
                return cnt;
            b = &ai_Blds[j];
            if (b->type != 0 && ai_Unk5c37a0(p, q, b) != 0
                && (!win || (r = gm_Liking(b->owner, *(unsigned short *)p, 1), lo <= r && r <= hi))) {
                out[cnt] = b->id;
                cnt++;
            }
            k--;
            j = (j + step) % 0x100;
        } while (cnt < n);
    }
    return cnt;
}
