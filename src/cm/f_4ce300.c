/* 0x4ce300, updates the bombs of cm_Bombs: a bomb older than 0x15e ticks explodes ("Explosion_s", the effects
   0x416340 / 0x4168e0 / 0x416040) and hurts the characters within 170.0 (bear hug action 0x60cb80); returns the number
   of used slots. Rough draft: the effect arguments are from the disassembly, not checked. */

int ai_Unk4ce300()
{
    float v[8];
    float pos[3];
    char col[4];
    float dx, dy, dz, dist;
    char *r;
    char **pp;
    char *rec, *c, *g;
    int cnt = 0;
    int n, t1, amt;
    unsigned int k;
    double d1;
    char *e;

    r = ai_Unk4cd9d0(0x174);
    for (rec = cm_Bombs; (int)rec < (int)(cm_Bombs + 0x100); rec += 8) {
        if (*(int *)rec == 0)
            continue;
        cnt++;
        if (d3_Time <= (unsigned int)(*(int *)(rec + 4) + 0x15e))
            continue;
        sb_PlaySampleI(ai_Unk6f0d14, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        v[0] = -243.0f;
        v[1] = -90.0f;
        v[2] = 190.0f;
        v[3] = 0;
        v[4] = -50.0f;
        v[5] = 0;
        v[6] = 38.0f;
        v[7] = 0;
        e = *(char **)rec;
        col[0] = 0xc0;
        col[1] = 0xc0;
        col[2] = 0xc0;
        col[3] = 0x3f;
        v[0] = *(float *)(e + 0x4c);
        v[1] = *(float *)(e + 0x50);
        v[2] = *(float *)(e + 0x54);
        d3t_CreateSmokeK(0, v, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, pos, 0x190, 6, *(int *)col);
        d3t_CreateEarthK(0, col, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, *(int *)col);
        d3t_CreateExplosionK(0, v, 0x1e, v, 120.0f, 80.0f, 20.0f, 60.0f, 0x3c);
        for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            c = *pp;
            if (c == 0)
                continue;
            g = *(char **)(c + 0x184);
            if (c[8] == 0)
                continue;
            e = *(char **)rec;
            dx = *(float *)(e + 0x4c) - *(float *)(*(char **)(g + 0x34) + 0x4c);
            dy = *(float *)(e + 0x50) - *(float *)(*(char **)(g + 0x34) + 0x50);
            dz = *(float *)(e + 0x54) - *(float *)(*(char **)(g + 0x34) + 0x54);
            dist = (float)sqrt(dz * dz + dy * dy + dx * dx);
            if (dist < 170.0f) {
                n = ai_Unk4e52f0((unsigned char)r[6] - (unsigned char)r[5]);
                k = n + (unsigned char)r[5];
                d1 = (double)k * *(float *)(c + 0x1c) * 0.01;
                t1 = (int)d1;
                amt = (int)(t1 * ((1.0 - dist * 0.0058823530562222f)));
                *(int *)(*pp + 0x24) -= amt;
                ai_Unk4cedc0(*pp, amt, *(int *)(*pp + 0x16c) == ai_Unk6f0cec ? 1 : 2);
                if (ai_Unk4d3390(c) == 0)
                    ch_QueueAnim(g, "kampf/bauch_greifen", 1);
            }
        }
        d3if_Delete((struct D3OBJECT *)*(int *)rec);
        *(int *)rec = 0;
    }
    return cnt;
}
