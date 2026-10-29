/* 0x4dada0, answers the session request m (kind 1: a vote between two characters, kind 2: a pick among candidates) of the player's
   record r: +0x94 the result, +0x98 the click, +0x9c a flag; the name at +0x14 cleared */

void ai_Unk4dada0(char *r, int *m)
{
    int c, x, y, res, click;

    *(int *)(r + 0x94) = -1;
    *(int *)(r + 0x98) = -1;
    if (*(int *)(r + 0x9c) != 0)
        return;
    c = gm_FindChar(*(int *)(r + 0x10));
    if (c == 0 || *(char *)(c + 0x166) == 0 || *(char *)(c + 0x1b1) != 0)
        return;
    if (m[0] == 1) {
        x = gm_FindChar(m[3]);
        y = gm_FindChar(m[4]);
        if (x == 0 || y == 0)
            return;
        res = ai_Unk4da9e0((char *)c, (char *)x, (char *)y, &click);
        *(int *)(r + 0x98) = click;
    } else {
        if (m[0] != 2)
            return;
        if (amt_CanTakeOffice(c, m[2]) == 0)
            return;
        res = ai_Unk4dab50((char)m[1], (char *)c, m + 3, &click);
        *(int *)(r + 0x98) = click;
    }
    *(int *)(r + 0x94) = res;
    sprintf(r + 0x14, "%s", &ch_EmptyName);
}
