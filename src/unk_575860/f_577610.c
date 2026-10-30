/* 0x577610, when `arg` is 0: 0x4eaa00 (1); when it is 1: finds the character record 0x770b80 + i * 0x21c whose
   byte +2 is 5, asks 0x4e7150 (rec, 1, &v) and, when it answers > 0, calls 0x4e6180 (rec, v) */

void ai_Unk577610(int arg)
{
    char *rec;
    int v;
    int p, i;

    if (arg == 0) {
        amt_Unk4eaa00(1);
        return;
    }
    if (arg != 1)
        return;
    for (p = ((unsigned)&ai_Chars + 0x2), i = 0; p < (unsigned)((unsigned)ai_CharsV + 0x65402); p += 0x21c, i++)
        if (*(unsigned char *)p == 5)
            break;
    if (p >= (unsigned)((unsigned)ai_CharsV + 0x65402))
        return;
    rec = (char *)((unsigned)&ai_Chars + i * 0x21c);
    if (amt_GetOfficeCandidates(rec, 1, &v) > 0)
        amt_ApplyForOffice(rec, v);
}
