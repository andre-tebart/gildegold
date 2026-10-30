/* 0x4a6ae0, execute callback of the cannon shot: fires the 1..3 balls the decide callback 0x4a67a0 planned
   into ai_Unk6f0698 (one ball from 50 units along the barrel, two or three side by side from 40) */

int ai_Unk4a6ae0(AIPER *p, AIBLD *b)
{
    float u, v;
    float pa[3], pb[3], pc[3];
    float f, c, g, s;
    float x0, x1, x2;
    char s1[104], s2[104], s3[104];
    int cost, x;

    if (ai_Unk6f0698.who == *(int *)((unsigned char *)p + 4) && ai_Unk6f0698.bld == b->id) {
        unk_Unk4ef260(b, &u, &v);
        g = unk_Unk4ef330(b);
        cost = ai_Unk6f0698.cost;
        x = ai_Unk6f0698.x;
        c = ai_Unk5edfe0[ai_Unk6f0698.kind].c;
        f = ai_Unk6f0698.k * 0.01f * (g * ai_Unk5edfe0[ai_Unk6f0698.kind].b - ai_Unk5edfe0[ai_Unk6f0698.kind].a)
            + ai_Unk5edfe0[ai_Unk6f0698.kind].a;
        if (ai_Unk6f0698.charge == 1) {
            pa[0] = ai_Unk6f0698.a[0] + ai_Unk6f0698.b[0] * 50.0f;
            pa[1] = ai_Unk6f0698.a[1] + ai_Unk6f0698.b[1] * 50.0f;
            pa[2] = ai_Unk6f0698.a[2] + ai_Unk6f0698.b[2] * 50.0f;
            ai_InitShot(s1, pa, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_ScatterShot(s1, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            unk_Unk4eeb70(b, p, s1, 0, 0, unk_Unk4eef50(ai_Unk6f0698.kind, ai_Unk6f0698.k, 1), x, cost);
            return 1;
        }
        if (ai_Unk6f0698.charge == 2) {
            x0 = ai_Unk6f0698.b[0] * 40.0f + ai_Unk6f0698.a[0];
            x1 = ai_Unk6f0698.a[1] + ai_Unk6f0698.b[1] * 40.0f;
            x2 = ai_Unk6f0698.b[2] * 40.0f + ai_Unk6f0698.a[2];
            pa[0] = ai_Unk6f0698.d[0] * 5.0f + x0;
            pa[1] = ai_Unk6f0698.d[1] * 5.0f + x1;
            pa[2] = ai_Unk6f0698.d[2] * 5.0f + x2;
            pb[0] = ai_Unk6f0698.d[0] * -5.0f + x0;
            pb[1] = ai_Unk6f0698.d[1] * -5.0f + x1;
            pb[2] = ai_Unk6f0698.d[2] * -5.0f + x2;
            ai_InitShot(s2, pa, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_InitShot(s1, pb, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_ScatterShot(s2, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            ai_ScatterShot(s1, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            unk_Unk4eeb70(b, p, s2, s1, 0, unk_Unk4eef50(ai_Unk6f0698.kind, ai_Unk6f0698.k, 2), x, cost);
            return 1;
        }
        if (ai_Unk6f0698.charge == 3) {
            x0 = ai_Unk6f0698.b[0] * 40.0f + ai_Unk6f0698.a[0];
            x1 = ai_Unk6f0698.b[1] * 40.0f + ai_Unk6f0698.a[1];
            x2 = ai_Unk6f0698.b[2] * 40.0f + ai_Unk6f0698.a[2];
            pc[0] = ai_Unk6f0698.c[0] * 5.0f + x0;
            pc[1] = ai_Unk6f0698.c[1] * 5.0f + x1;
            pc[2] = ai_Unk6f0698.c[2] * 5.0f + x2;
            s = ai_Unk6f0698.c[0] * -3.0f;
            pa[0] = ai_Unk6f0698.d[0] * 5.0f + x0 + s;
            pa[1] = ai_Unk6f0698.d[1] * 5.0f + ai_Unk6f0698.c[1] * -3.0f + x1;
            pa[2] = ai_Unk6f0698.d[2] * 5.0f + ai_Unk6f0698.c[2] * -3.0f + x2;
            pb[0] = ai_Unk6f0698.d[0] * -5.0f + s + x0;
            pb[1] = ai_Unk6f0698.d[1] * -5.0f + ai_Unk6f0698.c[1] * -3.0f + x1;
            pb[2] = ai_Unk6f0698.d[2] * -5.0f + ai_Unk6f0698.c[2] * -3.0f + x2;
            ai_InitShot(s3, pa, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_InitShot(s1, pb, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_InitShot(s2, pc, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_ScatterShot(s3, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            ai_ScatterShot(s1, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            ai_ScatterShot(s2, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            unk_Unk4eeb70(b, p, s3, s1, s2, unk_Unk4eef50(ai_Unk6f0698.kind, ai_Unk6f0698.k, 3), x, cost);
            return 1;
        }
    }
    return 0;
}
