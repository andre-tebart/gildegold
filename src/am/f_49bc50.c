/* 0x49bc50, execute callback 0x21: uses the object s1->unk04 of the character's list, scales four needs */

int ai_Unk49bc50(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->unk04);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = s2->unk04;
            u.unk08 = s2->unk08;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x21].w3[0].w = G5_ACTS[0x21].w1[0].w * g[3];
                    G5_ACTS[0x21].w3[1].w = G5_ACTS[0x21].w1[1].w * g[3];
                    G5_ACTS[0x21].w3[2].w = G5_ACTS[0x21].w1[2].w * g[3];
                    G5_ACTS[0x21].w3[3].w = G5_ACTS[0x21].w1[3].w * g[3];
                    return 0x21;
                }
                if (u.unk0c == -1) {
                    G5_ACTS[0x21].w3[0].w = -(G5_ACTS[0x21].w1[0].w * g[3] * 0.5f);
                    G5_ACTS[0x21].w3[1].w = -(G5_ACTS[0x21].w1[1].w * g[3] * 0.5f);
                    G5_ACTS[0x21].w3[2].w = -(G5_ACTS[0x21].w1[2].w * g[3] * 0.5f);
                    G5_ACTS[0x21].w3[3].w = -(G5_ACTS[0x21].w1[3].w * g[3] * 0.5f);
                }
                return 0x21;
            }
        }
    }
    return 0;
}
