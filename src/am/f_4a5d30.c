/* 0x4a5d30: as 0x4a5a30, but the partner comes from a random walk over all 768 characters (as 0x4a5b50) */

int ai_Unk4a5d30(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int len, c;
    int step, k, found, win;
    float r;

    step = ai_Unk5ed100[util_Random(0x10)];
    memset(q, 0, sizeof(q));
    win = 0;
    len = n * 2;
    if (len > 0x20)
        len = 0x20;
    memcpy(&q[7], ids, len);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[16] = 0;
        q[1] = fl;
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            *id = *(unsigned short *)ai_Partner;
            return 1;
        }
    }
    q[1] = fl;
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    k = 0x300;
    found = 0;
    c = util_Random(0x300);
    while (!found) {
        if (k == 0) {
            if (found)
                return 1;
            return 0;
        }
        if (ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)c, q, 0) != 0) {
            if (!win || (r = gm_Liking(c, *(unsigned short *)p, 1), lo <= r && r <= hi)) {
                found = 1;
                *id = (unsigned short)c;
            }
        }
        k--;
        c = (int)(c + step) % 0x300;
    }
    return 1;
}
