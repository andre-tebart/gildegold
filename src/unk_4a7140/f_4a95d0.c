/* 0x4a95d0, decide callback: p (not flagged 0x80000) picks a character 20..63 units away, into m (kind 7) */

int ai_Unk4a95d0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if ((p->flags1cc & 0x80000) != 0)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x497bc0);
    if (G10_4A58D0(p, 1, &flags, 20.0f, (float)util_Random(0x20) + 32.0f, &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
