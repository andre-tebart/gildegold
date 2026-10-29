/* 0x4dae90, like 0x4dada0 for a record of the other kind (the AI side: 0x4aab90 / 0x4aad30 decide instead of the session pickers); the out
   value is written through the first parameter slot */

void ai_Unk4dae90(char *r, int *m)
{
    char *p = r;
    int c, x, y, res;

    *(int *)(p + 0x94) = *(int *)(p + 0x98) = -1;
    if (*(int *)(p + 0x9c) != 0)
        return;
    c = gm_FindChar(*(int *)(p + 0x10));
    if (c == 0 || *(char *)(c + 0x166) == 0 || *(unsigned char *)(c + 0x1b1) > 0)
        return;
    if (m[0] == 1) {
        x = gm_FindChar(m[3]);
        y = gm_FindChar(m[4]);
        if (x == 0 || y == 0)
            return;
        res = ai_TrustedOfTwo(c, x, y, &r);
        *(char **)(p + 0x98) = r;
    } else {
        if (m[0] != 2)
            return;
        if (amt_CanTakeOffice(c, m[2]) == 0)
            return;
        res = ai_TrustedOfFour(*(unsigned char *)(m + 1), c, m + 3, &r);
        *(char **)(p + 0x98) = r;
    }
    *(int *)(p + 0x94) = res;
    strcpy(p + 0x14, &ch_EmptyName);
}
