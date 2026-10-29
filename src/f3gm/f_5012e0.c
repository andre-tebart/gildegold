/* 0x5012e0, frees the building b: a player's building (+0x27 a character of kind 6) tells 0x5a86c0 (its id); by building type: 0x1e frees the block at +0x71, 0x48 .. 0x4a the block at +0x65 through 0x4f0180, 0x4b .. 0x4d through the heap free (0x422620) and clears +0x65; the characters (0x770cec / 0x770cf0 per record) that point at it are cleared; its list at +0x5d is freed (0x500140) and the building type byte set to 0. Returns 0, -1 / -2 for no building / a free one. ROUGH */

int gm_FreeBuilding(char *b)
{
    int *q;
    char k;

    if (b == 0)
        return -1;
    if (*b == 0)
        return -2;
    if (*(unsigned short *)(b + 0x27) != 0xffff && ai_Chars[*(unsigned short *)(b + 0x27)][2] == 6)
        ai_Unk5a86c0(*(int *)(b + 1));
    k = *b;
    if (k == 0x1e) {
        if (*(int *)(b + 0x71) != 0) {
            mem_Free(*(int *)(b + 0x71));
            *(int *)(b + 0x71) = 0;
        }
        *(int *)(b + 0x71) = 0;
    } else if (k == 'H' || k == 'I' || k == 'J') {
        gv_FreeGraveyard(*(int *)(b + 0x65));
        *(int *)(b + 0x65) = 0;
    } else if (k == 'K' || k == 'L' || k == 'M') {
        mem_Free(*(int *)(b + 0x65));
        *(int *)(b + 0x65) = 0;
    }
    q = (int *)(ai_Chars[0] + 0x16c);
    do {
        if ((char *)q[1] == b)
            q[1] = 0;
        if ((char *)q[0] == b)
            q[0] = 0;
        q += 0x87;
    } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x16a));
    if (*(int *)(b + 0x5d) != 0)
        gm_FreeList((int *)(b + 0x5d));
    *b = 0;
    return 0;
}
