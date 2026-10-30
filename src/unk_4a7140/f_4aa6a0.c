/* 0x4aa6a0, decide callback: p (unk194 > 1) picks a random acquaintance within 20..80 units that 0x553d10
   accepts and it knows little (fewer than 2..5), into m (kind 7, its id at 0x10); in gets a count and a kind */

int ai_Unk4aa6a0(AIPER *p, unsigned char *in, unsigned char *m)
{
    int cnt[32];
    int ids[32];
    int n, j;
    unsigned int i;
    AIPER *o;

    if (p->unk194 > 1 && (n = unk_Unk4f3c50(ids, cnt, p->id)) != 0) {
        i = util_RandomU(n);
        j = n;
        do {
            o = AI_505DF0(ids[i]);
            if (o == 0)
                return 0;
            if (he_Unk553d10(p, o) == 0)
                return 0;
            if (gm_Liking(p->id, o->id, 1) < (double)util_RandomU(0x3d) + 20.0
                && cnt[i] < util_RandomU(4) + 2) {
                m[0] = 7;
                *(int *)(m + 4) = o->netId;
                *(unsigned int *)(m + 0x10) = o->id;
                *(unsigned int *)(in + 8) = util_RandomU(cnt[i]) + 1;
                *(unsigned int *)(in + 0x10) = util_RandomU(3);
                return 1;
            }
            i = (int)(i + 1) % n;
        } while (--j);
    }
    return 0;
}
