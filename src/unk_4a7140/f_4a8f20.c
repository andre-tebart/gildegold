/* 0x4a8f20, decide callback: two characters 33..100 units from p of which one is within 60 units of the other,
   into m (kind 7) */

int ai_Unk4a8f20(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags[2];
    unsigned short id[2];

    flags[0] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    flags[1] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (G10_4A58D0(p, 2, flags, 33.0f, (float)util_RandomU(0x18) + 77.0f, id) == 0)
        return 0;
    if (gm_Liking(id[0], id[1], 1) > 60.0f && gm_Liking(id[1], id[0], 1) > 60.0f)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id[0]].netId;
    *(int *)(m + 8) = ((AIPER *)ai_CharsT)[id[1]].netId;
    return 1;
}
