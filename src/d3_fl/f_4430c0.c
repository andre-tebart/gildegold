/* 0x4430c0, transforms, lights and projects the water points of a tile and queues its water polygons @deep B: read the whole 506-line asmdiff against the original 0x4430c0..0x4437f0 and diffclass: 518 of 580 hunk instructions are the same operations in a different register / stack-slot allocation (slots 34, regs 14, fpu 14; the tile lives in ebx in ours and esi in the original, the row list walked with different pointer and slot sets, the projection loops' local layouts all differ), plus two equivalent branch spellings (test cl,cl then jge vs jns). One meaning item found and fixed: the npolys min compared unsigned (jb) where the original is signed, now min(npolys, (int)(d3_SortMax - d3_SortCount)) -> jl at 0x4436ed. No missing statement/callee/constant remains; the rest needs vargen/try.py */

void d3fl_DrawWater(D3FLMAP *map, D3FLTILE *t)
{
    D3VECTOR4 sun;
    D3VECTOR4 p;
    D3FLWREG *r;
    D3FLREGION *reg;
    D3PVERT *v;
    D3FLPOLY *poly;
    D3CLIPINFO *clip;
    float *wv;
    float k, fx, h, f;
    unsigned int mask;
    int size, row, ym, x, idx, iv, npoints, npolys, i;
    unsigned char *water, *light;
    unsigned char a, l;

    if (map && t && t->lodLevel && t->water) {
        sun.x = d3fl_AxisY.x;
        sun.y = d3fl_AxisY.y;
        sun.z = d3fl_AxisY.z;
        d3_Normalize(&sun);
        size = map->size;
        water = map->water;
        mask = map->mask;
        npoints = t->nwpoints;
        v = (D3PVERT *)t->wpoints;
        light = map->light;
        npolys = t->nwpolys;
        k = 0.5f - (float)fabs(d3fl_Down.x * sun.x + d3fl_Down.y * sun.y + d3fl_Down.z * sun.z);
        for (r = t->wregions; r->y >= 0; r++) {
            ym = r->y & 3;
            row = (r->y & mask) * size;
            reg = &map->regions[(unsigned char)r->flag];
            p.x = d3fl_AxisZ.x * r->y + d3fl_Origin.x;
            p.y = d3fl_AxisZ.y * r->y + d3fl_Origin.y;
            p.z = d3fl_AxisZ.z * r->y + d3fl_Origin.z;
            fx = (float)r->x0;
            p.x = d3fl_AxisX.x * fx + p.x;
            p.y = d3fl_AxisX.y * fx + p.y;
            p.z = d3fl_AxisX.z * fx + p.z;
            h = k * reg->f14 + reg->flags.b[0];
            for (x = r->x0; x <= r->x1; x++, v++) {
                idx = (x & mask) + row;
                f = water[idx];
                wv = reg->wave[ym][x & 3];
                sun.x = d3fl_AxisY.x * f + wv[0];
                sun.y = d3fl_AxisY.y * f + wv[1];
                sun.z = d3fl_AxisY.z * f + wv[2];
                v->x = p.x + sun.x;
                v->y = p.y + sun.y;
                v->z = p.z + sun.z;
                a = (unsigned char)min(max(h + wv[3], 0.0f), 255.0f);
                if (reg->flags.b[2] & 1)
                    v->color = (a << 8 | a) << 8 | a;
                else {
                    l = light[idx];
                    f = (float)(l & 0x7f);
                    f += f;
                    if (l & 0x80) {
                        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_Ambient.x + d3fl_SunCol.x);
                        v->c.r = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_Ambient.y + d3fl_SunCol.y);
                        v->c.g = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_Ambient.z + d3fl_SunCol.z);
                    } else {
                        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_Ambient.x);
                        v->c.r = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_Ambient.y);
                        v->c.g = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_Ambient.z);
                    }
                    v->c.b = min(0xff, iv);
                    v->c.a = a;
                    v->specular = v->color;
                }
                p.x = d3fl_AxisX.x + p.x;
                p.y = d3fl_AxisX.y + p.y;
                p.z = d3fl_AxisX.z + p.z;
            }
        }
        d3_ClipMesh(t->clipCode, (D3PVERT *)t->wpoints, npoints, (D3PTRI *)t->wpolys, npolys);
        clip = (D3CLIPINFO *)d3_ClipInfo[t->clip & 0x3f];
        if (d3_FogOn) {
            for (i = npoints, v = (D3PVERT *)t->wpoints; i > 0; i--, v++)
                if (v->flags & 0x80) {
                    k = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * k + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * k + d3_CenterY;
                    v->rhw = k;
                    d3_Dist2 = v->x * v->x + v->y * v->y + v->z * v->z;
                    v->fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                        ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                }
        } else {
            for (i = npoints, v = (D3PVERT *)t->wpoints; i > 0; i--, v++)
                if (v->flags & 0x80) {
                    k = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * k + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * k + d3_CenterY;
                    v->rhw = k;
                }
        }
        d3_TriCount += npolys;
        d3_DrawnVerts += npoints;
        d3_VertCount += npoints;
        npolys = min(npolys, (int)(d3_SortMax - d3_SortCount));
        for (i = npolys, poly = t->wpolys; i > 0; i--, poly++)
            if (poly->flags < 0) {
                reg = &map->regions[poly->region];
                d3_SortPtr->item = poly;
                if (reg->tex2) {
                    d3_SortPtr->key = ((D3TEXTURE *)reg->tex2 - (D3TEXTURE *)d3_Textures) + 1;
                    reg->tex2->stamp = d3_FrameCount;
                } else
                    d3_SortPtr->key = 0;
                poly->tex = reg->tex2;
                poly->clip = clip;
                poly->uOfs = reg->uOfs;
                poly->vOfs = reg->vOfs;
                d3_SortPtr++;
                d3_SortCount++;
            }
    }
}
