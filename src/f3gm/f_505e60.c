/* 0x505e60, the level of the character id (0 for a free slot or an inactive one, 1 when 0x4e6f80 has no entry for its job byte +0x166): the record's byte + 1; below the maximum 10 and with no flag it is raised to the level of the master (+0x1a0, 0x505df0, once) when that is higher */

int gm_Unk505e60(unsigned id, int again)
{
    int r, k, l2;
    short *m;
    int rec[4];

    id &= 0xffff;
    if (*(int *)(ai_Chars[id] + 4) == -1 || ai_Chars[id][8] == 0)
        return 0;
    r = amt_GetKind(ai_Chars[id][0x166], rec);
    if (r == 0)
        return 1;
    k = (rec[2] & 0xff) + 1;
    if ((char)rec[2] == 10)
        return k;
    if (again != 0)
        return k;
    m = gm_FindChar(*(int *)(ai_Chars[id] + 0x60));
    if (m != 0) {
        l2 = gm_Unk505e60(*m, 1);
        if (k <= l2)
            return gm_Unk505e60(*m, 1);
    }
    return k;
}
