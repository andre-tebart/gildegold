/* 0x4a8d20, decide callback: p (unk194 >= 3) picks a character within 32..49 units, into m (kind 7) */

int ai_Unk4a8d20(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 < 3)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_Random(0x12) + 32.0f, &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_Chars)[id].netId;
    return 1;
}
