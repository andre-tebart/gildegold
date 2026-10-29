/* 0x4f4130, the count of 0x4f3fe0 without the lists (stops at 0x21) */

int unk_Unk4f4130(int p, unsigned t)
{
    int n, k, id;
    int *q;

    n = 0;
    if (gm_FindChar(p) == 0)
        return 0;
    if ((t & 0xffff) * 0x21c != -(unsigned)&ai_Chars) {
        id = *(int *)(ai_Chars[t & 0xffff] + 4);
        q = (int *)(unk_Tab91b6e0 + 4);
        while (q[-1] != id || (k = unk_Unk4f36e0(*q)) == -1 || *(int *)(gv_Graveyard + 0x16 + k * 0x2d) != p || (n++, n < 0x21)) {
            q += 2;
            if ((int)q > ((unsigned)unk_Tab91b6e0 + 0x4003))
                return n;
        }
        return n;
    }
    return 0;
}
