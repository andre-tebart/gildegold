/* 0x50a1c0, the same for the buildings of the owner o (+0x27 = o), optionally one building b and one character c */

int gm_Unk50a1c0(unsigned o, int b, int c, int *out)
{
    int n;
    char *it, *bl;

    n = 0;
    it = (char *)he_FindEvent(1, 0, 0x23);
    while (it != 0 && n < 9) {
        bl = (char *)gm_BuildQuery(1, 1, *(int *)(it + 0xac));
        if (bl != 0 && *(unsigned short *)(bl + 0x27) == o && (b == 0 || b == (int)bl) &&
            (c == 0xffff || *(int *)(ai_Chars[c] + 4) == *(int *)(it + 0xb0))) {
            n++;
            out[n - 1] = (int)it;
        }
        it = (char *)he_NextEvent();
    }
    return n;
}
