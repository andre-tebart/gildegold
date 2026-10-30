/* 0x4a8db0, decide callback: a building of the character 0x6f0468 (or one near p), and a random character whose
   building (0x16c) it is, into m (kind 7) */

int ai_Unk4a8db0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int k;
    unsigned int i;
    int n;
    AIBLD *b;

    if (ai_Partner != 0)
        ai_Unk4a63f0(AI_6F0468, 4, 0x1807d4190, 0, 100.0f, &k);
    if (k == -1) {
        if (ai_Unk4a63f0(p, 6, 0x1807d4190, 0, (float)util_RandomU(0x20) + 32.0f, &k) == 0)
            return 0;
    }
    b = gm_BuildQuery(1, 1, k);
    if (b == 0)
        return 0;
    i = util_RandomU(0x300);
    n = 0x300;
    while (((AIPER *)ai_CharsT)[i].id == 0xffff
           || *(AIBLD **)((unsigned char *)&((AIPER *)ai_CharsT)[i] + 0x16c) != b
           || ((AIPER *)ai_CharsT)[i].kind != 0) {
        if (--n == 0)
            return 0;
        i = (int)(i + 1) % 0x300;
    }
    if (&((AIPER *)ai_CharsT)[i] == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[i].netId;
    *(int *)(m + 8) = b->id;
    return 1;
}
