/* 0x503e80, resets the character state: 0x503c60 (i, 2) for all 0x300 characters, clears the 0x2b-int records from 0x7d5f80 to 0x7d6a40 (first word -1) and the three counters 0x6f1bc0 / 0x6f1bb8 / 0x6f1bbc */


void gm_ResetCharacters(void)
{
    int i;
    int *p, *p2, *q;

    i = 0;
    do {
        unk_Unk503c60(i, 2);
        i++;
    } while (i < 0x300);
    p = gm_PlayerRecords;
    do {
        p2 = p + 0x2b;
        q = p;
        for (i = 0x2b; i != 0; i--)
            *q++ = 0;
        *(short *)p = (short)0xffff;
        p = p2;
    } while ((int)p2 < (unsigned)((unsigned)gm_PlayerRecords + 0xac0));
    ai_CharCount = 0;
    ai_Unk6f1bb8 = 0;
    ai_Unk6f1bbc = 0;
}
