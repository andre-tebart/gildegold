/* 0x589e50, fills the building slots of the player (184 B): the table 0x6f5ec0 (0x589e00) gets slot 0 the market
   building 0x6f1bc8, slot 1 the town hall (the query 0x501190 (1, 5, 0xf) = the building type 0xf), the next slot
   the building at +0x170 of the first character of the kind 6 (the records 0x770b80 + i * 0x21c, at most 0x300),
   then from slot 3 the player's buildings (0x501190 (1, 4, i) / 0x501030, the word +0x27 = the character index)
   whose building kind record (0x7e3f30 + kind * 0x24d) byte 0 is not 2 */

unsigned char * gm_BuildNext(void); unsigned char * gm_BuildQuery(int n, ...);

void ai_Unk589e50(void)
{
    unsigned char *b;
    int i;
    int n;

    i = 0;
    b = (unsigned char *)((unsigned)&ai_Chars + 0x2);
    do {
        if (*b == 6)
            break;
        b += 0x21c;
        i++;
    } while ((int)b < (unsigned)((unsigned)ai_CharsV + 0x65402));
    n = 1;
    ai_Unk589e00(0, (char *)U_I((unsigned)&ai_MarketBuilding));
    b = (unsigned char *)gm_BuildQuery(1, 5, 0xf);
    if (b != 0)
        ai_Unk589e00(n++, (char *)b);
    b = (unsigned char *)U_I(((unsigned)&ai_Unk770cec + 0x4) + i * 0x21c);
    if (b != 0)
        ai_Unk589e00(n, (char *)b);
    n = 3;
    b = (unsigned char *)gm_BuildQuery(1, 4, i);
    while (b != 0) {
        if (*(unsigned char *)(U_I((unsigned)&ai_BTypes) + *b * 0x24d) != 2)
            ai_Unk589e00(n++, (char *)b);
        b = (unsigned char *)gm_BuildNext();
    }
}
