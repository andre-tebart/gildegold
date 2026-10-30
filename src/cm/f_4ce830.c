/* 0x4ce830, as 0x4ce300 for the grenades of ai_Unk935a00 (0x10 bytes: object, effect, cell x, cell y): the effect
   ends (flag 0x20 of its byte +0x2d), the cell of the map ai_Unk633738 loses 5 (and its mark is cleared), the
   characters within 100.0 are hurt (action "kampf/arm_greifen"). Rough draft. */

int ai_Unk4ce830()
{
    float v[8];
    float pos[3];
    char col[4];
    float dx, dy, dz, dist;
    char *r, *g;
    char **pp;
    char *rec, *c, *gg, *e;
    int cnt = 0;
    int n, t1, amt;
    unsigned int k;
    double d1;
    int *map;

    g = *(char **)(ai_Unk5fb5b0 + 0xb0);
    r = ai_Unk4cd9d0(0x176);
    for (rec = ai_Unk935a00; (int)rec < (int)(ai_Unk935a00 + 0x200); rec += 0x10) {
        if (*(int *)rec == 0)
            continue;
        cnt++;
        if ((*(char *)(*(char **)(rec + 4) + 0x2d) & 0x20) == 0)
            continue;
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
        d3t_CreateSmoke(0, v, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, pos, 0x190, 6, *(int *)col);
        d3t_CreateEarth(0, col, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, *(int *)col);
        d3t_CreateExplosion(0, v, 0x1e, 70.0f, 50.0f, 20.0f, 60.0f, 0x3c);
        map = (int *)ai_Unk633738;
        if (*(char *)(*(char **)(map + 5) + *(int *)(rec + 8) + *(int *)(rec + 0xc) * map[0]) != 0) {
            *(char *)(*(char **)(map + 4) + *(int *)(rec + 8) + *(int *)(rec + 0xc) * map[0]) -= 5;
            *(char *)(*(char **)(((int *)ai_Unk633738) + 5) + *(int *)(rec + 8) + *(int *)(rec + 0xc) * *(int *)ai_Unk633738) = 0;
            d3fl_MakeDivide(ai_Unk633738);
            d3fl_MakeLight(ai_Unk633738);
        }
        sb_PlaySample(ai_Unk6f0d14, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        for (pp = (char **)ai_Unk7603a0; (int)pp < (int)ai_Unk760420; pp++) {
            c = *pp;
            if (c == 0)
                continue;
            gg = *(char **)(c + 0x184);
            if (c[8] == 0)
                continue;
            d3sm_GetVertexI(g, *(int *)(rec + 8), *(int *)(rec + 0xc), v);
            dx = v[0] - *(float *)(*(char **)(gg + 0x34) + 0x4c);
            dy = v[1] - *(float *)(*(char **)(gg + 0x34) + 0x50);
            dz = v[2] - *(float *)(*(char **)(gg + 0x34) + 0x54);
            dist = (float)sqrt(dz * dz + dy * dy + dx * dx);
            if (dist < 100.0f) {
                n = ai_Unk4e52f0((unsigned char)r[6] - (unsigned char)r[5]);
                k = n + (unsigned char)r[5];
                d1 = (double)k * *(float *)(c + 0x1c) * 0.01;
                t1 = (int)d1;
                amt = (int)(t1 * (1.0 - dist * 0.01f));
                *(int *)(*pp + 0x24) -= amt;
                ai_Unk4cedc0(*pp, amt, *(int *)(*pp + 0x16c) == ai_Unk6f0cec ? 1 : 2);
                if (ai_Unk4d3390(c) == 0)
                    ch_QueueAnim(gg, "kampf/arm_greifen", 1);
            }
        }
        d3if_Delete((struct D3OBJECT *)*(int *)rec);
        *(int *)rec = 0;
    }
    return cnt;
}
