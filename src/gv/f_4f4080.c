/* 0x4f4080, as 0x4f3fe0 but the records' ids (+0) are stored */

int unk_Unk4f4080(int p, int *out, unsigned t)
{
    int n, k, i;
    int *q;
    int *o;

    n = 0;
    if (gm_FindChar(p) == 0)
        return 0;
    if ((t & 0xffff) * 0x21c != -(unsigned)&ai_Chars) {
        q = (int *)(unk_Tab91b6e0 + 4);
        o = out;
        for (i = 0x20; i != 0; i--)
            *o++ = -1;
        do {
            if (q[-1] == *(int *)(ai_Chars[t & 0xffff] + 4)) {
                k = unk_Unk4f36e0(*q);
                if (k != -1 && *(int *)(gv_Graveyard + 0x16 + k * 0x2d) == p) {
                    out[n] = *(int *)(gv_Graveyard + k * 0x2d);
                    n++;
                    if (n > 0x20)
                        return n;
                }
            }
            q += 2;
        } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
        return n;
    }
    return 0;
}
