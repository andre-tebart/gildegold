/* 0x50a150, the (up to 9) group (1, 0, 0x23) items of the character slot c (the owner id at +0xb0), optionally of the party p (+0xac) */

int gm_Unk50a150(int c, int p, int *out)
{
    int n;
    char *it;

    n = 0;
    it = (char *)he_FindEvent(1, 0, 0x23);
    while (it != 0 && n < 9) {
        if (*(int *)(it + 0xb0) == *(int *)(ai_Chars[c] + 4) && (p == 0 || *(int *)(it + 0xac) == *(int *)(p + 1))) {
            out[n] = (int)it;
            n++;
        }
        it = (char *)he_NextEvent();
    }
    return n;
}
