/* 0x577110, when `arg` is 0: finds the character record 0x770b80 + i * 0x21c whose owner (the pointer at
   0x770cec + i * 0x21c, +0x21c per record) has the player word 0x6f25b8 at +0x27, asks 0x55ea60 for two values and
   posts both (0x4c51b0 with 0x5f1890, 0x4c5150 with 0x631090) on the record's id +4; no return value */

void ai_Unk577110(int arg)
{
    char *rec;
    int a, b;
    int p, i;

    if (arg)
        return;
    for (p = (unsigned)&ai_Unk770cec, i = 0; p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a); p += 0x21c, i++) {
        rec = *(char **)p;
        if (rec && *(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId)
            break;
    }
    if (p >= ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a))
        return;
    rec = (char *)((unsigned)&ai_Chars + i * 0x21c);
    if (!he_Unk55ea60(rec, &a, &b))
        return;
    cm_RequestChrMoveToUniverse(*(int *)(rec + 4), a, b, (char *)(unsigned)"dummy_EINGANG");
    ai_Unk4c5150(*(int *)(rec + 4), a, b, (char *)(unsigned)&ch_EmptyName);
}
