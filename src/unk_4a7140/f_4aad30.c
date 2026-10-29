/* 0x4aad30, of the four candidates ids the one character p trusts most for district k (distance + 0x4e60b0, item
   0x2be +0.2, flag 0x400000 -0.2); a tie is broken at random; -1 if none */

int ai_TrustedOfFour(int k, AIPER *p, int *ids, int *out)
{
    float sc[4];
    AIPER *c[4];
    G10DIST info;
    float f, best;
    int tie, bi, i;
    unsigned int r;

    *out = 0;
    if (amt_GetOffice(k, &info) == 0)
        return -1;
    for (i = 0; i < 4; i++) {
        c[i] = AI_505DF0(ids[i]);
        if (c[i] == 0 || amt_GetOfficeRate(c[i], (unsigned char)info.lvl, &f) == 0) {
            sc[i] = -1000.0f;
        } else {
            sc[i] = gm_Liking(c[i]->id, p->id, 1) + f;
            if (ai_InventoryHas(c[i], 0x2be) != 0)
                sc[i] += 0.2f;
            if ((c[i]->flags1cc & 0x400000) != 0)
                sc[i] -= 0.2f;
        }
    }
    best = -10.0f;
    tie = 0;
    bi = -1;
    for (i = 0; i < 4; i++) {
        if (c[i] != 0) {
            if (sc[i] > best) {
                best = sc[i];
                tie = 0;
                bi = i;
            } else if (c[i] != 0 && best == sc[i]) {
                tie = 1;
            }
        }
    }
    if (bi < 0)
        return -1;
    if (!tie)
        return c[bi]->netId;
    r = util_Random(4);
    for (i = 4; i != 0; i--) {
        if (sc[r] == best && c[r] != 0) {
            if ((int)r >= 0)
                return c[r]->netId;
            return -1;
        }
        r = (r + 1) % 4;
    }
    return -1;
}
