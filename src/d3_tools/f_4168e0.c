/* 0x4168e0, creates the earth-brocken debris: count "Erdbrocken_an0" particles with the effect parameters
   a..d in v00 and e..g in v10, the base color in the color24 union member and flags = 1; returns the system */

D3PARTSYS *d3t_CreateEarth(void *parent, D3VECTOR *pos, int count, float size, float a, float b, float c,
                           float d, float e, float f, float g, D3COLOR color)
{
    D3PARTSYS *ps;
    D3TEXFLAGS flags;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 2;
    ps = par_Create(count, "Erdbrocken_an0", flags, size, parent, (D3PARTCB)d3t_EarthCB);
    if (ps) {
        par_SetPos(ps, pos);
        ps->v00.x = a;
        ps->v00.y = b;
        ps->v00.z = c;
        ps->v00.w = d;
        ps->v10.x = e;
        ps->v10.y = f;
        ps->v10.z = g;
        ps->flags = 1;
        ps->color24 = color;
    }
    return ps;
}
