/* 0x4a91a0, decide callback: on p's day, p without a partner (0x5c) and not in a list 0x53e400 record (0xac /
   0xb0) picks one of up to 12 characters of the other sex 30..100 units away (0x4a5ef0), into m (kind 7) */

int ai_Unk4a91a0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    unsigned short ids[12];
    AIJOB *r;
    unsigned int n;

    if (ai_TimeT.year % 4 == (p->netId & 3) && gm_FindChar(*(int *)((unsigned char *)p + 0x5c)) == 0) {
        for (r = he_FindEvent(1, 0, 0x41); r != 0; r = he_NextEvent()) {
            if (r->unkb0 == p->netId)
                return 0;
            if (r->unkac == p->netId)
                return 0;
        }
        n = ai_Unk4a5ef0(p, 12, ((unsigned)&ai_Unka69140 + 0x1982ca) - (((unsigned char *)p)[9] != 0), 30.0f, 100.0f, ids);
        if (n != 0) {
            m[0] = 7;
            *(int *)(m + 4) = ((AIPER *)ai_CharsT)[ids[util_Random(n)]].netId;
            return 1;
        }
    }
    return 0;
}
