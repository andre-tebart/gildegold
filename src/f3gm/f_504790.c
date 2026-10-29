/* 0x504790, sets the job byte (+0x164) of the character i to n: a changed job clears the int +0x174; for a player or priest (kinds 5 / 6 / 7) the old job is stored in the job record (0x506430, +0x74 + the group of the old job) and the new one when it is lower or the slot empty (flag 0x6f2318) */


void gm_SetJob(int i, unsigned n)
{
    char *c;
    int r, g;
    unsigned char b;
    char k;

    c = ai_Chars[i];
    if (n != (unsigned char)c[0x164])
        *(int *)(c + 0x174) = 0;
    r = gm_PlayerRecord(c);
    if (r != 0 && ((k = c[2], k == 6 || k == 7) || k == 5)) {
        g = gm_TitleGroup((unsigned char)c[0x164]);
        *(char *)(g + 0x74 + r) = c[0x164];
        g = gm_TitleGroup(n);
        b = *(unsigned char *)(g + 0x74 + r);
        if ((int)n < (int)b || b == 0)
            *(char *)(g + 0x74 + r) = (char)n;
        ai_Unk6f2318 = 1;
    }
    c[0x164] = (char)n;
}
