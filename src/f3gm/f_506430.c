/* 0x506430, the record (0x56 words each, 0x7d5f80 ..) of a player-like character c (kinds 5 / 6 / 7 whose word +0x50 has the top bit set; the low 4 bits give the slot), 0 else */

short *gm_PlayerRecord(char *c)
{
    char k;
    int u;

    k = c[2];
    if ((k == 6 || k == 7 || k == 5) && (short)*(unsigned short *)(c + 0x50) < 0 && (u = *(unsigned short *)(c + 0x50) & 0xf, u < 0x10))
        return (short *)gm_PlayerRecords + u * 0x56;
    return 0;
}
