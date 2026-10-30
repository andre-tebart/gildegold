/* 0x4a8ad0, decide callback: p (unk194 >= 3) picks a character of the other sex (0x4a58d0), into m (kind 7) with
   a random count 1..5 */

int ai_Unk4a8ad0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 < 3)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c2) - (((unsigned char *)p)[9] != 0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x40), &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    if (util_RandomU(p->unk194 / 2) > 1 && util_RandomU(p->unk194 / 2) > 4) {
        *(int *)(m + 8) = 5;
        return 1;
    }
    if (util_RandomU(p->unk194 / 2) > 1) {
        *(unsigned int *)(m + 8) = util_RandomU(p->unk194 / 2);
        return 1;
    }
    *(int *)(m + 8) = 1;
    return 1;
}
