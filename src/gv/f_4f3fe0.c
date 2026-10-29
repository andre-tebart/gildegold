/* 0x4f3fe0, fills out[0x20] (-1 first) with the indexes of the records of 0x91f700 that the character with the slot c (low word of t) owns of the graveyard player p: pairs (character id, record id) of 0x91b6e0 whose record belongs
   (+0x16) to p; returns the count (up to 0x21); 0 for no character or the slot 0 */

int unk_Unk4f3fe0(int p, int *out, unsigned t)
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
                    out[n] = k;
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
