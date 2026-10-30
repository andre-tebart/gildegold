/* 0x4a63f0: a building for p's action a: a random walk over the 256 buildings for one that 0x5c37a0 accepts
   with the class mask (bit 63 forced) and, when the window is narrower than 0..100, whose owner p rates
   (0x50c550) in [lo, hi]; its id into *out (-1 for none). am.h declares lo as int (c) */

int ai_Unk4a63f0(AIPER *p, int a, __int64 mask, int c, float f, int *out)
{
    unsigned int q[0x12];
    AIBLD *b;
    int step, n, found, win;
    unsigned int j;
    float r;

    step = ai_Unk5ed140[util_RandomU(0x10)];
    q[2] = (unsigned int)mask;
    q[4] = a;
    found = 0;
    q[3] = (unsigned int)(mask >> 32) | 0x80000000;
    win = 0;
    q[5] = 0;
    *out = -1;
    if ((*(float *)&c > 0.0f || f < 100.0f) && *(float *)&c < f)
        win = 1;
    n = 0x100;
    j = util_RandomU(0x100);
    while (!found) {
        if (n == 0) {
            if (found)
                return 1;
            return 0;
        }
        b = &ai_BldsT[j];
        if (b->type != 0 && ai_Unk5c37a0(p, q, b) != 0
            && (!win || (r = gm_Liking(b->owner, *(unsigned short *)p, 1), *(float *)&c <= r && r <= f))) {
            found = 1;
            *out = b->id;
        }
        n--;
        j = (j + step) % 0x100;
    }
    return 1;
}
