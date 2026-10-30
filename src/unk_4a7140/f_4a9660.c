/* 0x4a9660, decide callback: p (not flagged 0x40000, unk194 >= 8) picks a character within 18..33 units, into m
   (kind 7) */

int ai_Unk4a9660(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if ((p->flags1cc & 0x40000) != 0)
        return 0;
    if (p->unk194 < 8)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x497bc0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_Random(0x10) + 18.0f, &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
