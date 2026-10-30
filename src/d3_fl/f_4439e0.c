/* 0x4439e0, draws the floor: transforms the map into camera space; if the map is dirty, rebuilds the tile meshes
   (vertices, lit from the light map, and triangles; extra vertices on edges next to finer tiles), projects them and
   culls the triangles; then animates the water (water != 0) and queues the visible triangles of all tiles,
   rendering their cache textures on first use
   @deep B: full draft, a vargen 'ren' case: read the source and the 3800-line asmdiff (503 hunks, 2872 differing instructions of 4358, code 3987 / slots 340 / regs 22 / fill 9 per diffclass): the whole function differs by one register role - map is in esi in ours and edi in the original, so every [esi+off] shows as [edi+off] and the locals move to other frame slots (frame 0x14c ours vs 0x150) - plus fpu scheduling; the calls and their positions are equal, the loop structure and every struct offset match, no missing statement/constant/callee. Readability: Q2 d3_Unk633b1c (role: the scene sun/light colour, aliases d3if_Dir/d3o_Dir), D3FLTILE.unk38/unk40 (split point/poly counts) and D3FLTEX.unk58 (no evidence), Q4 2 raw *(T *) - deep work. Needs vargen/try.py */

/* ---- 0x4439e0..0x446990 (worker d3fl3, d3fl4) ---- */

/* light map value l (bit 7: lit by a point light, bits 0..6: sun shade) to the vertex colors of v */
#define D3FL_LIGHT(v, l)                                                        \
    f = (float)((l) & 0x7f);                                                    \
    f += f;                                                                     \
    if ((l) & 0x80) {                                                           \
        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_SunCol.x + d3fl_Ambient.x);   \
        (v)->c.r = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_SunCol.y + d3fl_Ambient.y);   \
        (v)->c.g = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_SunCol.z + d3fl_Ambient.z);   \
    } else {                                                                    \
        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_Ambient.x);                   \
        (v)->c.r = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_Ambient.y);                   \
        (v)->c.g = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_Ambient.z);                   \
    }                                                                           \
    (v)->c.b = min(0xff, iv);                                                   \
    (v)->specular = (v)->color;

/* world position of map point (X0, Z0) at height 0, as D3FL_CORNER */
#define D3FL_POS(p, X0, Z0)                                                     \
    p.x = d3fl_AxisX.x * (X0) + d3fl_Origin.x;                                  \
    p.y = d3fl_AxisX.y * (X0) + d3fl_Origin.y;                                  \
    p.z = d3fl_AxisX.z * (X0) + d3fl_Origin.z;                                  \
    p.x = d3fl_AxisZ.x * (Z0) + p.x;                                            \
    p.y = d3fl_AxisZ.y * (Z0) + p.y;                                            \
    p.z = d3fl_AxisZ.z * (Z0) + p.z;

#define T map->tiles[y][x]

void d3fl_Draw(D3FLMAP *map, char water)
{
    D3OBJECT *cam;
    D3FLTILE *t;
    D3PVERT *v, *v0, *v1, *v2;
    D3FLPOLY *poly, *np, *pp;
    D3FLSPLIT *sp;
    D3FLTEX *tex;
    D3FLCACHE *c;
    D3CLIPINFO *clip;
    D3VECTOR4 a, q, r, p, dx, dz;
    float *uv;
    float dx0, dy0, dz0, fx, fy, fs, h, f, u, w, k;
    unsigned int n, size, mask, x, y, i, j, s, half, nx, ny, mx, my, lx, ly, idx, npts, npolys;
    unsigned char *light, *types, *height, *div;
    int iv, jit;
    char changed, d, fl;
    void *surf;

    if (d3_Hardware && d3_Camera) {
        n = map->n;
        size = map->size;
        light = map->light;
        types = map->types;
        height = map->height;
        mask = map->idxMask;
        if (map->nolight) {
            d3fl_Ambient.x = 0.0f;
            d3fl_Ambient.y = 0.0f;
            d3fl_Ambient.z = 0.0f;
            d3fl_SunCol.x = 0.0f;
            d3fl_SunCol.y = 0.0f;
            d3fl_SunCol.z = 0.0f;
            d3fl_LightCol.x = 1.0f;
            d3fl_LightCol.y = 1.0f;
            d3fl_LightCol.z = 1.0f;
        } else {
            d3fl_Ambient.x = d3_AmbientColor.x;
            d3fl_Ambient.y = d3_AmbientColor.y;
            d3fl_Ambient.z = d3_AmbientColor.z;
            d3fl_SunCol.x = d3_Unk633b1c.x;
            d3fl_SunCol.y = d3_Unk633b1c.y;
            d3fl_SunCol.z = d3_Unk633b1c.z;
            d3fl_LightCol.x = map->ambient[0];
            d3fl_LightCol.y = map->ambient[1];
            d3fl_LightCol.z = map->ambient[2];
        }
        cam = d3_Camera;
        dx0 = map->orgx - cam->pos.x;
        dy0 = map->orgy - cam->pos.y;
        dz0 = map->orgz - cam->pos.z;
        d3fl_Origin.x = dx0 * cam->matrix._11 + dy0 * cam->matrix._21 + dz0 * cam->matrix._31;
        d3fl_Origin.y = dx0 * cam->matrix._12 + dy0 * cam->matrix._22 + dz0 * cam->matrix._32;
        d3fl_Origin.z = dx0 * cam->matrix._13 + dy0 * cam->matrix._23 + dz0 * cam->matrix._33;
        d3fl_AxisX.x = map->scale * cam->matrix._11 + map->xAxisY * cam->matrix._21 + map->xAxisZ * cam->matrix._31;
        d3fl_AxisX.y = map->scale * cam->matrix._12 + map->xAxisY * cam->matrix._22 + map->xAxisZ * cam->matrix._32;
        d3fl_AxisX.z = map->scale * cam->matrix._13 + map->xAxisY * cam->matrix._23 + map->xAxisZ * cam->matrix._33;
        d3fl_AxisZ.x = map->zAxisX * cam->matrix._11 + map->zAxisY * cam->matrix._21 + map->negScale * cam->matrix._31;
        d3fl_AxisZ.y = map->zAxisX * cam->matrix._12 + map->zAxisY * cam->matrix._22 + map->negScale * cam->matrix._32;
        d3fl_AxisZ.z = map->zAxisX * cam->matrix._13 + map->zAxisY * cam->matrix._23 + map->negScale * cam->matrix._33;
        d3fl_AxisY.x = map->yAxisX * cam->matrix._11 + map->hscale * cam->matrix._21 + map->yAxisZ * cam->matrix._31;
        d3fl_AxisY.y = map->yAxisX * cam->matrix._12 + map->hscale * cam->matrix._22 + map->yAxisZ * cam->matrix._32;
        d3fl_AxisY.z = map->yAxisX * cam->matrix._13 + map->hscale * cam->matrix._23 + map->yAxisZ * cam->matrix._33;
        if (map->dirty) {
            changed = d3fl_UpdateTiles(map);
            /* vertices and triangles */
            for (y = 0; y < 8; y++)
                for (x = 0; x < 8; x++) {
                    mx = x * n;
                    my = y * n;
                    t = &map->tiles[y][x];
                    if (!(s = t->lodLevel)) {
                        t->npolys = 0;
                        t->npoints = 0;
                    } else {
                        fx = (float)mx;
                        a.x = d3fl_AxisX.x * fx;
                        a.y = d3fl_AxisX.y * fx;
                        a.z = d3fl_AxisX.z * fx;
                        half = s >> 1;
                        fy = (float)my;
                        q.x = d3fl_AxisZ.x * fy;
                        q.y = d3fl_AxisZ.y * fy;
                        q.z = d3fl_AxisZ.z * fy;
                        a.x = q.x + a.x;
                        a.y = q.y + a.y;
                        a.z = q.z + a.z;
                        r.x = d3fl_Origin.x + a.x;
                        r.y = d3fl_Origin.y + a.y;
                        r.z = d3fl_Origin.z + a.z;
                        nx = ny = n / s + 1;
                        if (x == 7)
                            nx -= 4 / s;
                        if (y == 7)
                            ny -= 4 / s;
                        if (t->lodLevel != (unsigned char)t->lod || changed)
                            t->npoints = ny * nx;
                        fs = (float)s;
                        dx.x = d3fl_AxisX.x * fs;
                        dx.y = d3fl_AxisX.y * fs;
                        dx.z = d3fl_AxisX.z * fs;
                        dz.x = d3fl_AxisZ.x * fs;
                        dz.y = d3fl_AxisZ.y * fs;
                        dz.z = d3fl_AxisZ.z * fs;
                        v = (D3PVERT *)t->points;
                        lx = (my * size + mx) & mask;
                        for (j = 0; j < ny; j++) {
                            p.x = r.x;
                            p.y = r.y;
                            p.z = r.z;
                            idx = lx;
                            for (i = 0; i < nx; i++) {
                                h = height[idx];
                                a.x = d3fl_AxisY.x * h;
                                a.y = d3fl_AxisY.y * h;
                                a.z = d3fl_AxisY.z * h;
                                v->x = p.x + a.x;
                                v->y = p.y + a.y;
                                v->z = p.z + a.z;
                                D3FL_LIGHT(v, light[idx])
                                p.x += dx.x;
                                p.y += dx.y;
                                p.z += dx.z;
                                idx = (idx + s) & mask;
                                v++;
                            }
                            r.x += dz.x;
                            r.y += dz.y;
                            r.z += dz.z;
                            lx = (lx + s * size) & mask;
                        }
                        if (s > 0 && changed) {
                            v = (D3PVERT *)t->points;
                            poly = t->polys;
                            t->npolys = 0;
                            idx = (my / s * map->size / s + mx / s) & map->idxMask;
                            div = map->divide[half];
                            for (j = 0, ly = my; j < ny - 1; j++, ly += s) {
                                v0 = v;
                                v1 = v + nx;
                                for (i = 0, lx = mx; i < nx - 1; i++, lx += s) {
                                    d = div[idx + i];
                                    if (d & 0x40)
                                        jit = d3_Noise[(j + my) & 0xff][(i + mx) & 0xff] & 0x3f;
                                    else
                                        jit = 0;
                                    poly[0].tex = poly[1].tex = d3fl_GetTexture(types, size, lx, ly, s, map->src);
                                    if (d & 0x80) {
                                        poly[0].flip &= ~1;
                                        poly[0].v[2] = poly[1].v[0] = (D3FLPOINT *)(v0 + 1);
                                        poly[0].v[0] = (D3FLPOINT *)v1;
                                        poly[0].v[1] = (D3FLPOINT *)v0;
                                        poly[1].v[1] = (D3FLPOINT *)(v1 + 1);
                                        poly[1].v[2] = (D3FLPOINT *)v1;
                                        poly[0].uv = &d3fl_Jitter[jit * 24];
                                        poly[1].uv = &d3fl_Jitter[jit * 24 + 6];
                                    } else {
                                        poly[0].flip |= 1;
                                        poly[0].v[1] = poly[1].v[2] = (D3FLPOINT *)(v1 + 1);
                                        poly[1].v[1] = (D3FLPOINT *)(v0 + 1);
                                        poly[0].v[0] = poly[1].v[0] = (D3FLPOINT *)v0;
                                        poly[0].v[2] = (D3FLPOINT *)v1;
                                        poly[0].uv = &d3fl_Jitter[jit * 24 + 12];
                                        poly[1].uv = &d3fl_Jitter[jit * 24 + 18];
                                    }
                                    poly += 2;
                                    v0++;
                                    v1++;
                                    t->npolys += 2;
                                }
                                idx += map->size / s;
                                v += nx;
                            }
                        }
                    }
                }
            /* extra vertices on the edges next to finer tiles; their triangles are split in two */
            for (y = 0; y < 8; y++)
                for (x = 0; x < 8; x++) {
                    if (T.lodLevel && changed) {
                        s = T.lodLevel;
                        half = s >> 1;
                        ny = n / s + 1;
                        if (x == 7)
                            nx = ny - 4 / s;
                        else
                            nx = ny;
                        if (y == 7)
                            ny -= 4 / s;
                        mx = x * n;
                        my = y * n;
                        T.unk40 = 0;
                        T.unk38 = 0;
                        sp = T.split;
                        if (x < 7 && map->tiles[y][x + 1].lodLevel > 0 && map->tiles[y][x + 1].lodLevel < s) {
                            /* right edge */
                            lx = mx + n;
                            ly = my + half;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            uv = T.bp;
                            poly = T.polys + (nx - 2) * 2;
                            D3FL_POS(p, lx, ly)
                            fs = (float)s;
                            dz.x = d3fl_AxisZ.x * fs;
                            dz.y = d3fl_AxisZ.y * fs;
                            dz.z = d3fl_AxisZ.z * fs;
                            idx = (ly * size + mx + n) & mask;
                            for (j = 0; j < ny - 1; j++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->x = lx;
                                    sp->y = ly;
                                    D3FL_LIGHT(v, light[idx])
                                    D3FL_COPY_TO(uv, poly[1].uv, 0x18)
                                    if (poly->flip & 1) {
                                        u = (uv[2] + uv[4]) * 0.5f;
                                        w = (uv[3] + uv[5]) * 0.5f;
                                        uv[4] = u;
                                        uv[5] = w;
                                        D3FL_COPY_TO(np, &poly[1], 0x28)
                                        poly[1].v[2] = (D3FLPOINT *)v;
                                        poly[1].uv = uv;
                                        T.unk40++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    } else {
                                        u = (uv[2] + uv[0]) * 0.5f;
                                        w = (uv[3] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, &poly[1], 0x28)
                                        poly[1].v[0] = (D3FLPOINT *)v;
                                        poly[1].uv = uv;
                                        T.unk40++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    }
                                    uv += 6;
                                    sp++;
                                    np->zOfs = 16.0f;
                                    np++;
                                    T.unk40++;
                                    T.npolys++;
                                    v++;
                                    T.unk38++;
                                    T.npoints++;
                                }
                                p.x += dz.x;
                                p.y += dz.y;
                                p.z += dz.z;
                                idx += s * size;
                                ly += s;
                                poly += (nx - 1) * 2;
                            }
                        }
                        if (x && map->tiles[y][x - 1].lodLevel > 0 && map->tiles[y][x - 1].lodLevel < s) {
                            /* left edge */
                            ly = half + my;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            uv = (float *)((D3FLTRIUV *)T.bp + T.unk40);
                            poly = T.polys;
                            D3FL_POS(p, mx, ly)
                            fs = (float)s;
                            dz.x = d3fl_AxisZ.x * fs;
                            dz.y = d3fl_AxisZ.y * fs;
                            dz.z = d3fl_AxisZ.z * fs;
                            idx = (ly * size + mx) & mask;
                            for (j = 0; j < ny - 1; j++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->x = mx;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->y = ly;
                                    D3FL_LIGHT(v, light[idx])
                                    *(D3FLTRIUV *)uv = *(D3FLTRIUV *)poly->uv;
                                    if (poly->flip & 1) {
                                        u = (uv[4] + uv[0]) * 0.5f;
                                        w = (uv[5] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, poly, 0x28)
                                        poly->v[0] = (D3FLPOINT *)v;
                                        poly->uv = uv;
                                        T.unk40++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[4] = u;
                                        uv[5] = w;
                                        np->v[2] = (D3FLPOINT *)v;
                                    } else {
                                        u = (uv[2] + uv[0]) * 0.5f;
                                        w = (uv[3] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, poly, 0x28)
                                        poly->v[0] = (D3FLPOINT *)v;
                                        poly->uv = uv;
                                        T.unk40++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    }
                                    uv += 6;
                                    sp++;
                                    np->zOfs = 16.0f;
                                    np++;
                                    T.unk40++;
                                    T.npolys++;
                                    v++;
                                    T.npoints++;
                                    T.unk38++;
                                }
                                p.x += dz.x;
                                p.y += dz.y;
                                p.z += dz.z;
                                idx += s * size;
                                ly += s;
                                poly += (nx - 1) * 2;
                            }
                        }
                        if (y && map->tiles[y - 1][x].lodLevel > 0 && map->tiles[y - 1][x].lodLevel < s) {
                            /* top edge */
                            lx = half + mx;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            uv = (float *)((D3FLTRIUV *)T.bp + T.unk40);
                            poly = T.polys;
                            D3FL_POS(p, lx, my)
                            fs = (float)s;
                            dx.x = d3fl_AxisX.x * fs;
                            dx.y = d3fl_AxisX.y * fs;
                            dx.z = d3fl_AxisX.z * fs;
                            idx = (my * size + half + mx) & mask;
                            for (i = 0; i < nx - 1; i++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->x = lx;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->y = my;
                                    D3FL_LIGHT(v, light[idx])
                                    if (poly->flip & 1) {
                                        D3FL_COPY_TO(uv, poly[1].uv, 0x18)
                                        u = (uv[2] + uv[0]) * 0.5f;
                                        w = (uv[3] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, &poly[1], 0x28)
                                        poly[1].uv = uv;
                                        poly[1].v[0] = (D3FLPOINT *)v;
                                        T.unk40++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    } else {
                                        D3FL_COPY_TO(uv, poly->uv, 0x18)
                                        u = (uv[2] + uv[4]) * 0.5f;
                                        w = (uv[3] + uv[5]) * 0.5f;
                                        uv[4] = u;
                                        uv[5] = w;
                                        D3FL_COPY_TO(np, poly, 0x28)
                                        poly->uv = uv;
                                        poly->v[2] = (D3FLPOINT *)v;
                                        T.unk40++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    }
                                    uv += 6;
                                    sp++;
                                    np->zOfs = 16.0f;
                                    np++;
                                    T.unk40++;
                                    T.npolys++;
                                    v++;
                                    T.npoints++;
                                    T.unk38++;
                                }
                                p.x += dx.x;
                                p.y += dx.y;
                                p.z += dx.z;
                                idx += s;
                                lx += s;
                                poly += 2;
                            }
                        }
                        if (y < 7 && map->tiles[y + 1][x].lodLevel > 0 && map->tiles[y + 1][x].lodLevel < s) {
                            /* bottom edge */
                            lx = half + mx;
                            ly = n + my;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            D3FL_POS(p, lx, ly)
                            uv = (float *)((D3FLTRIUV *)T.bp + T.unk40);
                            fs = (float)s;
                            dx.x = d3fl_AxisX.x * fs;
                            dx.y = d3fl_AxisX.y * fs;
                            dx.z = d3fl_AxisX.z * fs;
                            poly = T.polys + (ny - 2) * (nx - 1) * 2;
                            idx = (ly * size + half + mx) & mask;
                            for (i = 0; i < nx - 1; i++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->x = lx;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->y = ly;
                                    D3FL_LIGHT(v, light[idx])
                                    pp = poly;
                                    if (!(poly->flip & 1))
                                        pp = poly + 1;
                                    D3FL_COPY_TO(uv, pp->uv, 0x18)
                                    u = (uv[2] + uv[4]) * 0.5f;
                                    w = (uv[3] + uv[5]) * 0.5f;
                                    uv[4] = u;
                                    uv[5] = w;
                                    D3FL_COPY_TO(np, pp, 0x28)
                                    pp->uv = uv;
                                    pp->v[2] = (D3FLPOINT *)v;
                                    T.unk40++;
                                    uv += 6;
                                    D3FL_COPY_TO(uv, np->uv, 0x18)
                                    np->uv = uv;
                                    uv[2] = u;
                                    uv[3] = w;
                                    uv += 6;
                                    np->v[1] = (D3FLPOINT *)v;
                                    np->zOfs = 16.0f;
                                    T.unk40++;
                                    T.npolys++;
                                    v++;
                                    sp++;
                                    T.npoints++;
                                    T.unk38++;
                                    np++;
                                }
                                p.x += dx.x;
                                p.y += dx.y;
                                p.z += dx.z;
                                poly += 2;
                                idx += s;
                                lx += s;
                            }
                        }
                    }
                    if (T.lodLevel > 0 && T.unk38 > 0 && !changed)
                        /* same levels: move the edge vertices */
                        for (i = 0, sp = T.split; i < T.unk38; i++, sp++) {
                            h = height[sp->y * size + sp->x];
                            fx = (float)sp->x;
                            p.x = d3fl_AxisX.x * fx + d3fl_Origin.x;
                            p.y = d3fl_AxisX.y * fx + d3fl_Origin.y;
                            p.z = d3fl_AxisX.z * fx + d3fl_Origin.z;
                            fy = (float)sp->y;
                            q.x = d3fl_AxisZ.x * fy + p.x;
                            q.y = d3fl_AxisZ.y * fy + p.y;
                            q.z = d3fl_AxisZ.z * fy + p.z;
                            ((D3PVERT *)sp->p)->x = d3fl_AxisY.x * h + q.x;
                            ((D3PVERT *)sp->p)->y = d3fl_AxisY.y * h + q.y;
                            ((D3PVERT *)sp->p)->z = d3fl_AxisY.z * h + q.z;
                        }
                    T.lod = T.lodLevel;
                }
            /* clipping, projection and culling */
            for (y = 0, t = map->tiles[0]; y < 8; y++)
                for (x = 0; x < 8; x++, t++)
                    if (t->lodLevel) {
                        npts = t->npoints;
                        npolys = t->npolys;
                        d3_ClipMesh(t->clip, t->points, npts, t->polys, npolys);
                        if (d3_FogOn) {
                            for (i = 0, v = (D3PVERT *)t->points; i < npts; i++, v++)
                                if (v->flags < 0) {
                                    k = 1.0f / v->z;
                                    v->sx = k * v->x * d3_ScaleX + d3_CenterX;
                                    v->sy = k * v->y * d3_ScaleY + d3_CenterY;
                                    v->rhw = k;
                                    d3_Dist2 = v->x * v->x + v->y * v->y + v->z * v->z;
                                    v->fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                                        ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                                }
                        } else {
                            for (i = 0, v = (D3PVERT *)t->points; i < npts; i++, v++)
                                if (v->flags < 0) {
                                    k = 1.0f / v->z;
                                    v->sx = k * v->x * d3_ScaleX + d3_CenterX;
                                    v->sy = k * v->y * d3_ScaleY + d3_CenterY;
                                    v->rhw = k;
                                }
                        }
                        for (i = 0, poly = t->polys; i < npolys; i++, poly++)
                            if ((fl = poly->flags) < 0) {
                                v2 = (D3PVERT *)poly->v[2];
                                v1 = (D3PVERT *)poly->v[1];
                                v0 = (D3PVERT *)poly->v[0];
                                if ((v2->flags | v1->flags | v0->flags) & 0x10)
                                    fl |= 0x40;
                                else if ((v0->sx - v1->sx) * (v0->sy - v2->sy) >= (v0->sx - v2->sx) * (v0->sy - v1->sy))
                                    fl &= 0xbf;
                                else
                                    fl &= 0x7f;
                                poly->flags = fl;
                            }
                    }
            map->dirty = 0;
        }
        if (water)
            d3fl_UpdateWater(d3_Time, map->nregions, map->regions);
        /* queue the visible triangles */
        for (y = 0, t = map->tiles[0]; y < 8; y++)
            for (x = 0; x < 8; x++, t++)
                if (t->lodLevel) {
                    if (t->zmin <= d3_MinZ)
                        d3_MinZ = t->zmin;
                    if (t->zmax >= d3_MaxZ)
                        d3_MaxZ = t->zmax;
                    clip = (D3CLIPINFO *)d3_ClipInfo[t->clip & 0x3f];
                    d3_TriCount += t->npolys;
                    d3_VertCount += t->npoints;
                    d3_DrawnVerts += t->npoints;
                    npolys = min((unsigned int)t->npolys, d3_SortMax - d3_SortCount);
                    for (i = 0, poly = t->polys; i < npolys; i++, poly++)
                        if (poly->flags < 0) {
                            if ((tex = poly->tex) && tex->name[0] == '*' && !(c = tex->cache)->flag) {
                                tex->unk58 = 0;
                                if (!tex->surf)
                                    tex_Prepare(tex);
                                if (surf = dd_LockSurfaceFlags(tex->surf, 0x20)) {
                                    d3fl_Render(surf, &c->t);
                                    dd_UnlockSurface(tex->surf);
                                    if (d3fl_Mips)
                                        d3d_BuildMipmaps(tex->surf, 0);
                                    c->flag = 1;
                                }
                            }
                            d3_SortPtr->item = poly;
                            if (tex) {
                                d3_SortPtr->key = ((D3TEXTURE *)tex - (D3TEXTURE *)d3_Textures) + 1;
                                tex->stamp = d3_FrameCount;
                            } else
                                d3_SortPtr->key = 0;
                            poly->clip = clip;
                            d3_SortPtr++;
                            d3_SortCount++;
                        }
                    d3fl_DrawWater(map, t);
                }
    }
}
