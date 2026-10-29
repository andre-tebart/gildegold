/* 0x49b050, execute callback 0x1e: uses the object s1->unk04 of the character's list */

int ai_Unk49b050(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->unk04);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = -1;
            u.unk08 = -1;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x1e].w3[0].w = G5_ACTS[0x1e].w1[0].w * g[3];
                    return 0x1e;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x1e].w3[0].w = -(G5_ACTS[0x1e].w1[0].w * g[3]);
                return 0x1e;
            }
        }
    }
    return 0;
}
