/* 0x520200, reads the mission table 0x6f0724 / 0x601058 .. 0x60106c, 0xf83520 (0x10 bytes), and the 0x11-word records from 0xf83548 (8 pairs of words each). Returns 1 @deep diff read against the source and the original: meaning verified - the 1-byte hst_TextsOn read, ai_Unk4b1d20, the two 4-byte counts (hst_TextCurrent / hst_KeyNext = the first + n), the 16 bytes at 0xf83520, the record loop p = 0xf83548 with the 8 word pairs read from p-4 / p and p += 0x11 to > 0xf83657 - all match. The 100 code hunks are the known epilogue shape (the original inlines pop pop pop pop ret after every failed read, ours shares one return-0 block) plus register/order scheduling (inc ebx before/after add esi, xor ebx placement). Readability: Q1, Q2 ai_Unk4b1d20 (also Q7: undeclared), Q3 the 0xf83520 / 0xf83548 / 0xf83657 image literals. */


int unk_Unk520200(int f)
{
    int n, i, j;
    int *p, *q;

    LS_R(&hst_TextsOn, 1)
    ai_Unk4b1d20();
    LS_R(&n, 4)
    hst_TextCurrent = hst_TextFirst + n;
    if (ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    hst_KeyNext = hst_KeyFirst + n;
    for (i = 0; i < 0x10; i++)
        LS_R((char *)(unsigned)&hst_Key + i, 1)
    p = (int *)((unsigned)&hst_Lists + 0x8);
    while (ls_Read((int)(p - 2), 4, 1, f) != 0) {
        q = p;
        for (j = 0; j < 8; j++) {
            LS_R(q - 1, 4)
            LS_R(q, 4)
            q += 2;
        }
        p += 0x11;
        if ((int)p > ((unsigned)((unsigned)hst_Lists + 0x114) + 0x3))
            return 1;
    }
    return 0;
}
