/* 0x505f30, the same for the byte +0x167 */

int gm_Unk505f30(unsigned id, int again)
{
    int r, k, l2;
    short *m;
    int rec[4];

    id &= 0xffff;
    if (*(int *)(ai_Chars[id] + 4) == -1 || ai_Chars[id][8] == 0)
        return 0;
    r = amt_GetKind(ai_Chars[id][0x167], rec);
    if (r == 0)
        return 1;
    k = (rec[2] & 0xff) + 1;
    if ((char)rec[2] == 10)
        return k;
    if (again != 0)
        return k;
    m = gm_FindChar(*(int *)(ai_Chars[id] + 0x60));
    if (m != 0) {
        l2 = gm_Unk505f30(*m, 1);
        if (k <= l2)
            return gm_Unk505f30(*m, 1);
    }
    return k;
}
