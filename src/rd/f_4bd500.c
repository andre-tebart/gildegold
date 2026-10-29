/* 0x4bd500, marks the area of building site name (its four corners, 0x4bd760) @deep behaviour: far draft, 160 code hunks - frame 0x6c vs 0x70 and the corner positions copied through the FPU in the original (fld/fstp) where our float array copies are integer moves, plus scheduling of the four rd_WorldToCell calls and the rd_FillQuad call; the four corners and the call sequence were read and match in meaning */

void rd_AllBauplatzToSupermap(G15ROAD *r, char *name)
{
    G15BK *bk;
    float v[3];
    float c[8];
    char buf[0x40];

    bk = rd_GetBKSize(name);
    if (bk == 0) {
        sprintf(buf, "rd_AllBauplatzToSupermap(): Unbekannter Bauplatz '%s'", name);
        return;
    }
    v[0] = bk->x0;
    v[1] = bk->y0;
    v[2] = bk->z0;
    rd_WorldToCell(&c[0], &c[1], v, name, r);
    v[0] = bk->x1;
    v[1] = bk->y0;
    v[2] = bk->z0;
    rd_WorldToCell(&c[2], &c[3], v, name, r);
    v[0] = bk->x1;
    v[1] = bk->y0;
    v[2] = bk->z1;
    rd_WorldToCell(&c[4], &c[5], v, name, r);
    v[0] = bk->x0;
    v[1] = bk->y0;
    v[2] = bk->z1;
    rd_WorldToCell(&c[6], &c[7], v, name, r);
    rd_FillQuad(r, c, -1, 0);
}
