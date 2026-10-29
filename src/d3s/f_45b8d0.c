/* 0x45b8d0, adds the shadow of slot to the shadow object @deep B code hunks 572 (510 asm lines): frame 0x54 vs 0x3c (the original keeps 24 more bytes of locals - colour components / rect / uv floats in memory), esi/ebp/edi hold material, buf and slot->tex in other roles; the six calls (4x _ftol 0x5de518 for the four colour casts, d3s_TerrainShadowMesh, d3s_MeshFromMap) agree and the sampled hunks are frame / register differences. Q2 unk05c/unk090: the same fields are named color 0x05c and range/power/atten 0x090 in d3_light.h's D3LOBJ view (a d3.h field rename is project-wide and not done here) */

char d3s_AddShadow(D3SSLOT *slot, void *grid)
{
    D3SBUF *buf;
    D3SPOLY *p;
    D3SVERT *v;
    float *pos;
    float u0, v0, du, dv;
    int rect[4];
    int x0, x1, z0, z1, w, h, n;
    union { unsigned long d; D3COLOR c; } col;
    unsigned long color;
    char ok = 0;
    D3TEXHANDLE *tex;

    buf = (D3SBUF *)((char *)d3s_Object->material + 0xf4);
    tex = slot->tex;
    col.c.r = (unsigned char)slot->light->unk05c.x;
    col.c.g = (unsigned char)slot->light->unk05c.y;
    col.c.b = (unsigned char)slot->light->unk05c.z;
    col.c.a = (unsigned char)(slot->light->unk090.y * 0.5f);
    color = col.d;
    if (d3s_Enabled && slot->map) {
        if (slot->size && !slot->simple) {
            x0 = slot->ix0;
            if (x0 >= slot->size || (x1 = slot->ix1) < 0 || (z0 = slot->iz0) >= slot->size
                || (z1 = slot->iz1) < 0 || x1 - x0 <= 0 || z1 - z0 <= 0)
                return 0;
            w = x1 - x0;
            h = z1 - z0;
            du = 1.0f / w;
            u0 = 0.0f;
            dv = 1.0f / h;
            if (x0 < 0) {
                u0 = -x0 / (float)w;
                x0 = 0;
            }
            v0 = 0.0f;
            if (x1 >= slot->size)
                x1 = slot->size - 1;
            if (z0 < 0) {
                v0 = -z0 / (float)h;
                z0 = 0;
            }
            if (z1 >= slot->size)
                z1 = slot->size - 1;
            h = z1 - z0;
            w = x1 - x0;
            if ((unsigned int)(h * 2 * w + buf->npolys) >= d3s_MaxPolys * 2)
                return 0;
            n = (h + 1) * (w + 1);
            if ((unsigned int)(buf->nverts + n) >= (d3s_MaxPolys >> 4) * 25)
                return 0;
            rect[0] = x0;
            rect[1] = x1;
            rect[2] = z0;
            rect[3] = z1;
            if (!grid)
                return d3s_MeshFromMap(rect, slot->size, slot->map, &slot->origin.x, &slot->cell.x, &u0, buf, tex, color);
            if ((unsigned int)((h / ((int *)grid)[1] + 1) * w + buf->nverts + (w / ((int *)grid)[1] + 1) * h + n) < (d3s_MaxPolys >> 4) * 25)
                return d3s_TerrainShadowMesh(grid, rect, &u0, buf, tex, color);
            return 0;
        }
    }
    if ((unsigned int)buf->npolys < d3s_MaxPolys * 2 - 2) {
        p = &buf->polys[buf->npolys];
        v = &buf->verts[buf->nverts];
        p[0].v[0] = v;
        p[0].uv = d3s_QuadUV;
        p[0].v[2] = v + 2;
        p[0].v[1] = v + 1;
        p[0].tex = tex;
        p[1].v[1] = v + 3;
        p[1].v[2] = v + 2;
        p[1].v[0] = v + 1;
        p[1].uv = d3s_QuadUV2;
        p[1].tex = tex;
        buf->npolys += 2;
        pos = &d3s_Verts[d3s_NumVerts * 6];
        v[0].pos = pos;
        pos[0] = slot->x0;
        pos[1] = slot->y;
        pos[2] = slot->z1;
        v[0].c0 = color;
        v[0].c1 = color;
        v[1].pos = pos + 6;
        pos[6] = slot->x1;
        pos[7] = slot->y;
        pos[8] = slot->z1;
        v[1].c0 = color;
        v[1].c1 = color;
        v[2].pos = pos + 12;
        pos[12] = slot->x0;
        pos[13] = slot->y;
        pos[14] = slot->z0;
        v[2].c0 = color;
        v[2].c1 = color;
        v[3].pos = pos + 18;
        pos[18] = slot->x1;
        pos[19] = slot->y;
        pos[20] = slot->z0;
        v[3].c0 = color;
        v[3].c1 = color;
        ok = 1;
        buf->nverts += 4;
        d3s_NumVerts += 4;
    }
    return ok;
}
