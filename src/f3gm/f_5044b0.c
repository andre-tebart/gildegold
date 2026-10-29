/* 0x5044b0, the job byte (+0x164) of the character i when its group (0x504ad0) is p, else the byte of the job record of the character (0x506430, +0x74 + p) or 0 */


unsigned char gm_Unk5044b0(int i, int p)
{
    int k, r;

    k = gm_TitleGroup(ai_Chars[i][0x164]);
    if (k == p)
        return ai_Chars[i][0x164];
    r = gm_PlayerRecord(ai_Chars[i]);
    if (r == 0)
        return 0;
    return *(unsigned char *)(r + 0x74 + p);
}
