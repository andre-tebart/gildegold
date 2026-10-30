/* 0x4a9420, decide callback: p (unk194 >= 2) picks a character within 40..71 units that 0x553d60 accepts, into
   m (kind 7) */

int ai_Unk4a9420(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 < 2)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_Random(0x20) + 40.0f, &id) == 0)
        return 0;
    if (he_Unk553d60(p, &((AIPER *)ai_CharsT)[id]) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
