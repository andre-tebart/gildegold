/* 0x45f2b0, the map cell under the screen point (sx, sy): steps along the view ray @deep behaviour: the two meaning errors found are fixed (loop cell read now rounds like the original: iz/ix = (int)(z+0.5f)/(int)(x+0.5f) with the 0.5f constant at 0x5eb7ac, index iz*h->size+ix instead of the transposed ix*h->size+iz, and *px/*pz use the same rounded values as 45f5cc-45f5ff; asmdiff 239 -> 219 lines); what remains is codegen: frame sub esp 0x80 vs 0x68 and the whole local slot layout, fld x; fcomp limit; test ah,0x5; jp in the bound checks vs our fld limit; fmul 1.75; fcomp x; and eax,0x100; je (operand order), step*dx/dy/dz hoisted once in the original (fst [esp+0xc]; fmul) vs our per-iteration fmul, fdivr 1.0 vs fld 1.0/fdiv, and x87 stack order; readability already readable */

char d3sm_Pick(D3SM *h, int sx, int sy, int *px, int *pz)
{
    D3MATRIX m;
    float x, y, z, dx, dy, dz, fx, fy, step, lim;
    int ix, iz;

    if (!h)
        return 0;
    x = (d3_Camera->pos.x - h->origin.x) / h->cell.x;
    y = (d3_Camera->pos.y - h->origin.y) / h->cell.y;
    z = (d3_Camera->pos.z - h->origin.z) / h->cell.z;
    fx = (sx - d3_CenterX) / d3_ScaleX;
    fy = (sy - d3_CenterY) / d3_ScaleY;
    d3_MatTranspose(&d3_Camera->matrix, &m);
    dx = (m._11 * fx + m._21 * fy + m._31) / h->cell.x;
    dy = (m._12 * fx + m._22 * fy + m._32) / h->cell.y;
    dz = (m._13 * fx + m._23 * fy + m._33) / h->cell.z;
    if (fabs(dx) < d3_SnapEpsilon && fabs(dz) < d3_SnapEpsilon) {
        ix = (int)x;
        iz = (int)z;
        if (dy > 0.0f && h->data[h->size * iz + ix] < y)
            return 0;
        *px = ix;
        *pz = iz;
        return 1;
    }
    step = 1.0f / max(fabs(dx), fabs(dz));
    lim = -h->size * 0.75f;
    while (x >= lim) {
        if (x >= h->size * 1.75f || z <= -h->size * 1.75f || z >= h->size * 1.75f)
            return 0;
        if (x >= 0.0f && x < h->size && z >= 0.0f && z < h->size) {
            iz = (int)(z + 0.5f);
            ix = (int)(x + 0.5f);
            if (h->data[iz * h->size + ix] >= y) {
                *px = (int)(x + 0.5f);
                *pz = (int)(z + 0.5f);
                return 1;
            }
        }
        x += step * dx;
        y += step * dy;
        z += step * dz;
    }
    return 0;
}
