/* 0x4aa580, decide callback: p (unk194 >= 1) picks a character within 0..63 units that 0x553db0 / 0x570ea0
   accept, with fewer than 4 (0x553db0's count, by chance), into m (kind 7) */

int ai_Unk4aa580(AIPER *p, int c, unsigned char *m)
{
    int k;
    int flags;
    unsigned short id;

    k = 0;
    if (p->unk194 < 1)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x40), &id) == 0)
        return 0;
    if (he_Unk553db0(p, &((AIPER *)ai_CharsT)[id], &k) == 0)
        return 0;
    if (he_Unk570ea0(&((AIPER *)ai_CharsT)[id], p) == 0)
        return 0;
    if (k > 3)
        return 0;
    if (util_RandomF() < k * 0.25f)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
