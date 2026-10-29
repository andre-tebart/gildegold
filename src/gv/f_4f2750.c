/* 0x4f2750, the message 0x4c5930 (character id +4 of c or -1, byte i, int v clamped to the table record i's limits rec[1] .. rec[2]); -1 for a bad record or a dead character */
int gv_SetTableRecord(short *c, int i, int v)
{
    int rec[9];
    int m[3];
    int lim;

    if (gv_GetTableRecord(i, rec) == 0)
        return -1;
    if (c == 0)
        m[0] = -1;
    else {
        if (*c == -1)
            return -1;
        m[0] = *(int *)(c + 2);
    }
    lim = rec[1];
    if (v < rec[1] || (lim = rec[2], rec[2] < v))
        v = lim;
    *(char *)(m + 1) = (char)i;
    *(int *)((char *)m + 5) = v;
    return ((int (*)(char *))ai_Unk4c5930)(m);
}
