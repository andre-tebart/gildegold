/* 0x4a8c80, decide callback: two characters near p (0x4a58d0), into m (kind 7) */

int ai_Unk4a8c80(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags[2];
    unsigned short id[2];

    flags[0] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    flags[1] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 2, flags, 0, (float)util_Random(0x40), id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_Chars)[id[0]].netId;
    *(int *)(m + 8) = ((AIPER *)ai_Chars)[id[1]].netId;
    return 1;
}
