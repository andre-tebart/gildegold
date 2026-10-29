/* 0x4a1000, eval callback: not for kind-5 characters' partners of kind 5 (or busy, 0x47dd70); the oldest (0x0a) of
   the five children 0x68.. who is grown up (0x0a + 4 >= 0x20), unmarried (0x164) and has no job 0x5e; rated
   only when that child is old enough */

int ai_Unk4a1000(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c, *best;
    int m, w;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    if (((unsigned char *)p)[2] != 5) {
        c = (G5CH *)gm_FindChar(*(int *)((unsigned char *)p + 0x5c));
        if (c != 0) {
            if (((unsigned char *)c)[2] == 5)
                return 0;
            if (((unsigned char *)c)[9] == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(((int *)((unsigned char *)p + 0x68))[m]);
        if (c != 0 && ((unsigned char *)c)[8] > 0
            && (float)(c->unk00a + 4) >= *(float *)((unsigned char *)c + 0x20)
            && ((unsigned char *)c)[0x164] == 0 && he_FindEvent(2, 2, c->id, 0, 0x5e) == 0
            && (best == 0 || c->unk00a > best->unk00a))
            best = c;
    }
    if (best == 0)
        return 0;
    *a = 0.0f;
    *b = 0.0f;
    ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
    w = (float)(best->unk00a + 4) >= *(float *)((unsigned char *)best + 0x20) ? 1 : 0;
    *a = w * *a;
    *b = w * *b;
    return 1;
}
