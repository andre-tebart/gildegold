/* 0x45af50, the shadow mesh over the part rect of a terrain with 8x8 blocks, each block sampled at its own lod @deep B code hunks 738 with the frame 0xa4 here vs 0xa0: the prologue and loops read out as the same loads / FP maths on other stack slots (every local of the 0xa0 note slot map in another home, the hoisted fstep*uv / fstep*cx / fstep*cz products and fx0 / oy+1.0 in other slots); no missing statement or call found in the hunks read; the note's slot map is what a deep round needs (notes/units/d3s.md [q3] Attempts) */

char d3s_TerrainShadowMesh(D3STERR *g, int *rect, float *uv, D3SBUF *buf, D3TEXHANDLE *tex, unsigned long color)
{
    D3VECTOR4 o, p;
    float d[4];
    float cx, cy, cz;
    unsigned char *heights, *mip, *mp;
    unsigned int stride, step, rowskip;
    int x, z, zend, xend, xi, zi, k, r, nrows, ncx;
    int bz, bz1, bx, bx0, bx1, nz;
    float vcur, ucur, fstep, cxs, czs, fxc, vtop, vbot, ul, ur;
    float *pos, *uvp;
    D3SVERT *vert, *vp, *tr, *bl, *br, *trow, *brow;
    D3SPOLY *poly;
    char drew;
    unsigned char lod;

    o.x = g->origin.x;
    o.y = g->origin.y;
    o.z = g->origin.z;
    cx = g->axisx.x;
    cy = g->axisy.y;
    heights = g->heights;
    cz = g->axisz.z;
    stride = g->size;
    bz1 = rect[3] / (int)g->bs;
    drew = 0;
    bx1 = rect[1] / (int)g->bs;
    bx0 = rect[0] / (int)g->bs;
    vcur = uv[2];
    z = rect[2];
    for (bz = z / (int)g->bs, nz = bz + 1; bz <= bz1; bz++, nz++) {
        if (bz == bz1)
            zend = rect[3];
        else
            zend = nz * g->bs;
        x = rect[0];
        ucur = uv[0];
        for (bx = bx0; bx <= bx1; ) {
            if (bz >= 0 && bz < 8 && bx >= 0 && bx < 8 && (lod = g->blocks[bz][bx].lod) > 0) {
                step = lod;
                rowskip = stride / step;
                fstep = (float)step;
                drew = 1;
                mip = g->mip[step >> 1];
                d[1] = fstep * uv[1];
                d[3] = fstep * uv[3];
                cxs = cx * fstep;
                czs = fstep * cz;
                if (bx == bx1)
                    xend = rect[1];
                else
                    xend = (bx + 1) * g->bs;
                p.x = (float)x * cx + o.x;
                poly = buf->polys + buf->npolys;
                pos = d3s_Verts + d3s_NumVerts * 6;
                p.y = o.y + 1.0f;
                vert = buf->verts + buf->nverts;
                uvp = d3s_UVs + d3s_NumUVs * 6;
                p.z = (float)z * cz + o.z;
                vp = vert;
                for (zi = z; zi <= zend; zi += step) {
                    for (xi = x, fxc = p.x; xi <= xend; xi += step) {
                        pos[0] = fxc;
                        pos[1] = (float)heights[zi * stride + xi] * cy + p.y;
                        pos[2] = p.z;
                        vp->c1 = color;
                        vp->pos = pos;
                        pos += 6;
                        vp++;
                        d3s_NumVerts++;
                        buf->nverts++;
                        fxc += cxs;
                    }
                    p.z += czs;
                }
                nrows = (zend - z) / step;
                vtop = vcur;
                ncx = (xend - x) / step;
                mp = mip + x / step + z / step * rowskip;
                for (r = 0, trow = vert + 1, brow = vert + (ncx + 1); r < nrows; r++) {
                    ul = ucur;
                    tr = trow;
                    bl = brow;
                    for (k = 0; k < ncx; k++) {
                        vbot = d[3] + vtop;
                        br = bl + 1;
                        poly->v[2] = bl;
                        if (mp[k] & 0x80) {
                            poly->v[1] = tr;
                            uvp[3] = vtop;
                            ur = ul + d[1];
                            poly[1].v[1] = br;
                            poly[1].v[0] = tr;
                            poly[1].v[2] = bl;
                            uvp[10] = ul;
                            uvp[6] = ur;
                            uvp[9] = vbot;
                        } else {
                            poly->v[1] = br;
                            uvp[3] = vbot;
                            poly[1].v[0] = tr - 1;
                            poly[1].v[1] = tr;
                            poly[1].v[2] = br;
                            uvp[9] = vtop;
                            ur = ul + d[1];
                            uvp[6] = ul;
                            uvp[10] = ur;
                        }
                        poly[1].flags &= 0xfd;
                        poly->flags &= 0xfd;
                        poly[1].uv = uvp + 6;
                        poly[1].z = -1.0f;
                        poly->z = -1.0f;
                        poly->v[0] = tr - 1;
                        poly->uv = uvp;
                        poly[1].tex = tex;
                        poly->tex = tex;
                        uvp[1] = vtop;
                        poly += 2;
                        uvp[2] = ur;
                        uvp[0] = ul;
                        uvp[4] = ul;
                        uvp[5] = vbot;
                        bl++;
                        uvp[7] = vtop;
                        tr++;
                        uvp[8] = ur;
                        uvp[11] = vbot;
                        d3s_NumUVs += 2;
                        uvp += 12;
                        buf->npolys += 2;
                        ul = ur;
                    }
                    mp += rowskip;
                    vtop += d[3];
                    brow += ncx + 1;
                    trow += ncx + 1;
                }
            }
            bx++;
            ucur = (float)(bx * (int)g->bs - x) * uv[1] + ucur;
            x = (x + (int)g->bs) / (int)g->bs * g->bs;
        }
        vcur = (float)(nz * (int)g->bs - z) * uv[3] + vcur;
        z = (z + (int)g->bs) / (int)g->bs * g->bs;
    }
    return drew;
}
