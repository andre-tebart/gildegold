/* 0x4a67a0: aims p's cannon in building b at building t: the cannon type r (0x4ef170 / 0x4ef000, p's 0x194 must
   reach its level), a ballistic solution (0x5dc1a0 / 0x5dc210 / 0x5dd100) within -15..45 degrees, its strength
   (0x4eef50) within the charge limit of the cannon and its cost (0x4ef090) within a quarter of the money; the
   shot goes into ai_Unk6f0698 */

int ai_Unk4a67a0(AIPER *p, AIBLD *b, AIBLD *t)
{
    G9CANNON *cn;
    G9KIND kind;
    float pa[4];
    float pb[4];
    float rng[4];
    float v94[2];
    float vb[4];
    float vc[4];
    float vd[4];
    float x5c, x48;
    int par[5];
    int r, r2, n, cost, x, k;
    float f, ang;

    ai_Unk6f0698.who = -1;
    ai_Unk6f0698.bld = -1;
    cn = gv_FindEffect(b->id);
    if (cn == 0)
        return 0;
    if (ai_Unk4a66f0(b, pa, t, pb) != 0)
        return 0;
    unk_Unk4ef170(b, &r2, &r);
    r = util_Random(r + 1);
    unk_Unk4ef000(r, &kind);
    if (*(int *)((unsigned char *)p + 0x194) < kind.level)
        return 0;
    f = unk_Unk4ef330(b);
    r2 = util_Random(r2 + 1);
    ai_SphereVolume(ai_Unk5eee08, ai_Unk5edfe0[r].c, par);
    ai_PositionShare(ai_Unk5edfe0[r].a, f * ai_Unk5edfe0[r].b, par, v94);
    rng[0] = v94[0];
    rng[1] = v94[1];
    rng[2] = -0.2617994f;
    rng[3] = 0.7853982f;
    if (ai_Unk5dd100(pa, pb, rng, &x5c, &x48, &ang, vb, vc, vd) != 0)
        return 0;
    k = (int)((ang - rng[0]) * 100.0f / (rng[1] - rng[0]));
    if (unk_Unk4eef50(r, k, r2 + 1) > cn->charge[r2])
        return 0;
    n = unk_Unk4ef090(p, r, k, r2 + 1, &x, &cost);
    if (n == 0)
        return 0;
    if ((int)(gm_Unk509e30(*(int *)((unsigned char *)p + 0x178), ai_OwnPlayer) * 0.25f) < cost)
        return 0;
    ai_Unk6f0698.a[0] = pa[0];
    ai_Unk6f0698.a[1] = pa[1];
    ai_Unk6f0698.a[2] = pa[2];
    ai_Unk6f0698.b[0] = vb[0];
    ai_Unk6f0698.b[1] = vb[1];
    ai_Unk6f0698.b[2] = vb[2];
    ai_Unk6f0698.c[0] = vc[0];
    ai_Unk6f0698.c[1] = vc[1];
    ai_Unk6f0698.c[2] = vc[2];
    ai_Unk6f0698.d[0] = vd[0];
    ai_Unk6f0698.d[1] = vd[1];
    ai_Unk6f0698.d[2] = vd[2];
    ai_Unk6f0698.who = *(int *)((unsigned char *)p + 4);
    ai_Unk6f0698.bld = b->id;
    ai_Unk6f0698.cost = cost;
    ai_Unk6f0698.charge = r2 + 1;
    ai_Unk6f0698.x = x;
    ai_Unk6f0698.ang = ang;
    ai_Unk6f0698.kind = r;
    ai_Unk6f0698.k = k;
    memcpy(ai_Unk6f0698.par, par, sizeof(par));
    return 1;
}
