/* 0x4bd760, fills the quad c (four cells x / z) on the town map with ground type v (-1: not) and on the grid with
   type t (0xff: not), two steps per cell
   @deep behaviour: far draft, 220 code hunks - the FPU block order differs (the original keeps c[0] / c[1] on the stack and loads c[2] / c[3] before the subtracts, ours computes dx/dz/a/b in another order) and 6 slot hunks; the two-step quad fill with the two sqrt(...)+0.5)*2 steps was read and matches in meaning */

void rd_FillQuad(G15ROAD *r, float *c, char v, int t)
{
    float dx, dz, ex, ez;
    float a, b, s;
    float px, pz, qx, qz;
    float fx, fz, x, z;
    int n, m;

    dx = c[2] - c[0];
    dz = c[3] - c[1];
    a = dz * dz + dx * dx;
    ex = c[4] - c[6];
    ez = c[5] - c[7];
    b = ez * ez + ex * ex;
    n = (int)(sqrt(a > b ? a : b) + 0.5) * 2;
    if (n < 1)
        n = 1;
    s = 1.0f / n;
    dx *= s;
    dz *= s;
    ex *= s;
    ez *= s;
    px = c[0];
    pz = c[1];
    qx = c[6];
    qz = c[7];
    for (; n > 0; n--) {
        fx = qx - px;
        fz = qz - pz;
        m = (int)(sqrt(fz * fz + fx * fx) + 0.5) * 2;
        if (m < 1)
            m = 1;
        s = 1.0f / m;
        fx *= s;
        fz *= s;
        x = px;
        z = pz;
        for (; m > 0; m--) {
            if (v != -1)
                r->map->cells[(int)z * r->map->w + (int)x] = v;
            if (t != 0xff)
                r->grid->cells[(r->map->w * (int)z + (int)x) * 7] = t;
            x += fx;
            z += fz;
        }
        px += dx;
        pz += dz;
        qx += ex;
        qz += ez;
    }
}
