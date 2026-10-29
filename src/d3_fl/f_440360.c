/* 0x440360, finds the water tile type ("WASSER"), builds the water height map, its regions and the water
   strips, points and polygons of the tiles
   @deep B: code 1285 / slots 200 / regs 6 (asmdiff 957 differing) - the function was read in round 4 (notes/units/d3_fl.md: complete draft, the same 36 calls in the same order, ours 16 bytes longer; the open code hunks are the ramp loop's v == 0xff retest threaded differently and the divide switch code built in a byte register in the original vs ebx in ours; everything else is local slot assignment, the diff tail is the misaligned disassembly after the length difference) - the same operations in another slot/register spelling, no missing statement or wrong constant; needs vargen/try.py. Readability: D3FLREGION/D3FLPOLY unk fields (header comments give wavePhase, the mod-1 texture offsets, bit 7 of unk24) and 2 raw *(T *) accesses */

void d3fl_PrepareWater(D3FLMAP *map)
{
    char msg[256];
    char buf[64];
    unsigned char *mask, *cmask, *c, *w, *w2;
    D3FLREGION *regs;
    D3FLTEX *tex;
    D3FLTILE *t;
    D3FLWREG *r;
    D3FLPOLY *p;
    D3TEXFLAGS f;
    int oldn, i, j, x, y, k, n, x0, x1, y1, np, npoly, first, left, off;
    unsigned char found, in, v, h0, h1;
    int tx, ty;

    oldn = map->nregions;
    map->nregions = 0;
    if (map->watertype == 0xff)
        for (i = 0; i < 8; i++) {
            strcpy(buf, map->typenames[i]);
            _strupr(buf);
            if (strstr(buf, "WASSER")) {
                map->watertype = i;
                break;
            }
        }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            map->tiles[j][i].water = 0;
    if (map->watertype < 0xff) {
        found = 0;
        mask = mem_Alloc(map->size * map->size, "d3_fl:Water(water_mask)");
        for (y = 0; y < map->size - 4; y++)
            for (x = 0; x < map->size - 4; x++) {
                k = map->size * y + x;
                if (map->types[k] == map->watertype) {
                    found = 1;
                    ty = y / map->n;
                    tx = x / map->n;
                    map->tiles[ty][tx].water = 1;
                    if (y % map->n == 0 && y > 0)
                        map->tiles[ty - 1][tx].water = 1;
                    if (x % map->n == 0 && x > 0)
                        map->tiles[ty][tx - 1].water = 1;
                    if (x % map->n == 0 && x > 0 && y % map->n == 0 && y > 0)
                        map->tiles[ty - 1][tx - 1].water = 1;
                    mask[k] = 0xff;
                    mask[map->size * y + ((x - 1) & map->mask)] = 0xff;
                    mask[map->size * y + ((x + 1) & map->mask)] = 0xff;
                    mask[((y - 1) & map->mask) * map->size + ((x - 1) & map->mask)] = 0xff;
                    mask[((y - 1) & map->mask) * map->size + x] = 0xff;
                    mask[((y - 1) & map->mask) * map->size + ((x + 1) & map->mask)] = 0xff;
                    mask[((y + 1) & map->mask) * map->size + ((x - 1) & map->mask)] = 0xff;
                    mask[((y + 1) & map->mask) * map->size + x] = 0xff;
                    mask[((y + 1) & map->mask) * map->size + ((x + 1) & map->mask)] = 0xff;
                }
            }
        if (!found)
            map->watertype = 0xff;
        else {
            if (map->water) {
                for (i = 0; i < map->size * map->size; i++) {
                    map->water[i] &= mask[i];
                    if (mask[i] == 0xff && !map->water[i])
                        map->water[i] = 0xff;
                }
            } else {
                map->water = mem_Alloc(map->size * map->size, "d3_fl:Water(height)");
                D3_COPY(map->water, mask, map->size * map->size);
            }
            /* ramps from the shore height into the water, row by row */
            for (y = 0; y < map->size; y++) {
                in = 0;
                for (x = 0; x < map->size; x++) {
                    k = map->size * y + x;
                    v = map->water[k];
                    if (v == 0xff && !in) {
                        in = 1;
                        x0 = x;
                        if (x > 0 && map->water[k - 1] > 0)
                            h0 = max(1, map->water[k - 1]);
                        else
                            h0 = max(1, map->height[k] - 2);
                    } else if (v != 0xff && in) {
                        in = 0;
                        if (v > 0)
                            h1 = max(1, v);
                        else
                            h1 = max(1, map->height[k - 1] - 2);
                        d3fl_Ramp(&map->water[map->size * y], x0, x - 1, h0, h1);
                    }
                }
                if (in) {
                    h1 = max(1, map->height[(y + 1) * map->size - 1] - 2);
                    d3fl_Ramp(&map->water[map->size * y], x0, map->size - 1, h0, h1);
                }
            }
            /* diagonals of the quads at the shore */
            for (y = 0; y < map->size; y++)
                for (x = 0; x < map->size; x++)
                    switch ((((map->types[map->size * y + x] == map->watertype) * 2
                              | map->types[((y + 1) & map->mask) * map->size + x] == map->watertype) * 2
                             | map->types[map->size * y + ((x + 1) & map->mask)] == map->watertype) * 2
                            | map->types[((y + 1) & map->mask) * map->size + ((x + 1) & map->mask)] == map->watertype) {
                    case 2:
                    case 4:
                    case 6:
                    case 11:
                    case 13:
                        map->divide[0][map->size * y + x] &= 0x7f;
                        break;
                    case 1:
                    case 7:
                    case 8:
                    case 9:
                    case 14:
                        map->divide[0][map->size * y + x] |= 0x80;
                        break;
                    }
            /* regions: connected quads with water at all four corners */
            cmask = mem_Alloc(map->size * map->size, "d3_fl:Water(CalcRegions)");
            D3FL_FILL(cmask, 0xff, map->size * map->size);
            for (y = 0; y < map->size - 1; y++)
                for (x = 0; x < map->size - 1; x++) {
                    c = &cmask[map->size * y + x];
                    if (*c == 0xff) {
                        w = &map->water[map->size * y + x];
                        w2 = &map->water[(y + 1) * map->size + x];
                        if (*w > 0 && *w2 > 0 && w[1] > 0 && w2[1] > 0)
                            *c = 0xfe;
                    }
                }
            for (y = 0; y < map->size - 1; y++)
                for (x = 0; x < map->size - 1; x++)
                    if (cmask[map->size * y + x] == 0xfe)
                        d3fl_Fill((char *)cmask, map->size, x, y, 0xfe, map->nregions++);
            if (map->nregions != oldn) {
                regs = mem_Alloc(map->nregions * sizeof(D3FLREGION), "d3_fl:Water(Regions)");
                if (oldn > map->nregions)
                    D3FL_COPY_TO(regs, map->regions, map->nregions * sizeof(D3FLREGION))
                else {
                    first = 0;
                    if (oldn > 0) {
                        D3FL_COPY_TO(regs, map->regions, oldn * sizeof(D3FLREGION));
                        first = oldn;
                    }
                    if (!(tex = tex_Find("EF_WASS_06A_2T_W_AN0"))) {
                        *(int *)&f = 0;
                        f.b[0] = 0xac;
                        if (tex = (D3FLTEX *)tex_Load("EF_WASS_06A_2T_W_AN0", f, 0, 0))
                            tex_Reload((D3TEXHANDLE *)tex, 0);
                    }
                    for (i = first; i < map->nregions; i++) {
                        regs[i].tex = regs[i].tex2 = tex;
                        if (tex) {
                            regs[i].flags = tex->flags;
                            if (i < map->nregions - 1)
                                tex_AddRef(tex, 1);
                        } else
                            *(int *)&regs[i].flags = 0;
                        regs[i].f14 = 62.0f;
                        regs[i].speed[0] = 0.023f;
                        regs[i].speed[1] = 0.02f;
                        regs[i].speed[2] = 0.027f;
                        regs[i].speed[3] = 0.03f;
                        regs[i].amp[0] = 1.0f;
                        regs[i].amp[1] = 1.0f;
                        regs[i].amp[2] = 1.0f;
                        regs[i].amp[3] = 31.0f;
                        regs[i].scrollSpeedU = 0;
                        regs[i].scrollSpeedV = 0;
                        regs[i].time = d3_Time;
                        regs[i].vOfs = 0;
                        regs[i].uOfs = 0;
                        regs[i].wavePhase[0] = 0;
                        regs[i].wavePhase[1] = 0;
                        regs[i].wavePhase[2] = 0;
                        regs[i].wavePhase[3] = 0;
                    }
                }
                if (map->regions) {
                    mem_Free(map->regions);
                    map->regions = 0;
                }
                map->regions = regs;
            }
            /* water strips, points and polygons of the tiles */
            for (j = 0; j < 8; j++)
                for (i = 0; i < 8; i++)
                    if (map->tiles[j][i].water) {
                        t = &map->tiles[j][i];
                        if (!t->wregions)
                            t->wregions = mem_Alloc(map->n * map->n / 2 * sizeof(D3FLWREG), "d3_fl:Water(strips)");
                        r = t->wregions;
                        np = 0;
                        npoly = 0;
                        y1 = j == 7 ? map->size - 4 : (j + 1) * map->n;
                        for (y = map->n * j; y <= y1; y++) {
                            in = 0;
                            x1 = i == 7 ? map->size - 4 : (i + 1) * map->n;
                            n = map->n;
                            for (x = n * i; x <= x1; x++) {
                                k = (y & map->mask) * map->size + (x & map->mask);
                                v = map->water[k];
                                if (in) {
                                    if (!v) {
                                        in = 0;
                                        r->x1 = x - 1;
                                        if (r->x0 < r->x1) {
                                            np += r->x1 - r->x0 + 1;
                                            r++;
                                        }
                                    }
                                } else if (v > 0 && ((y < (j + 1) * n && x < (i + 1) * n && cmask[k] < 0xff)
                                                     || (y > n * j && cmask[k - map->size] < 0xff)
                                                     || (x > n * i && cmask[k - 1] < 0xff))) {
                                    in = 1;
                                    r->y = y;
                                    r->x0 = x;
                                    r->idx = np;
                                    r->flag = 0xff;
                                }
                                n = map->n;
                                if (y < (j + 1) * n && x < (i + 1) * n && cmask[k] < 0xff)
                                    npoly += 2;
                            }
                            if (in) {
                                r->x1 = (i + 1) * map->n;
                                if (r->x0 < r->x1) {
                                    np += r->x1 - r->x0 + 1;
                                    r++;
                                }
                            }
                        }
                        r->y = -1;
                        if (t->wpoints && t->nwpoints != np) {
                            mem_Free(t->wpoints);
                            t->wpoints = 0;
                        }
                        t->nwpoints = np;
                        if (!t->wpoints)
                            t->wpoints = mem_Alloc(np * sizeof(D3FLPOINT), "d3_fl:Water(points)");
                        else
                            D3_ZERO(t->wpoints, np * sizeof(D3FLPOINT));
                        for (k = 0; k < t->nwpoints; k++) {
                            t->wpoints[k].src = 0;
                            t->wpoints[k].flags = 0;
                            t->wpoints[k].specular = 0x7fffffff;
                            t->wpoints[k].color = 0x7fffffff;
                        }
                        if (t->wpolys && t->nwpolys != npoly) {
                            mem_Free(t->wpolys);
                            t->wpolys = 0;
                        }
                        t->nwpolys = npoly;
                        if (!t->wpolys)
                            t->wpolys = mem_Alloc(npoly * sizeof(D3FLPOLY), "d3_fl:Water(polys)");
                        else
                            D3_ZERO(t->wpolys, npoly * sizeof(D3FLPOLY));
                        p = t->wpolys;
                        for (y = map->n * j; y < (j + 1) * map->n; y++)
                            for (x = map->n * i; x < (i + 1) * map->n; x++) {
                                k = (y & map->mask) * map->size + (x & map->mask);
                                if (cmask[k] < 0xff) {
                                    p->region = cmask[k];
                                    p->tex = 0;
                                    p->vOfs = 0;
                                    p->uOfs = 0;
                                    p->flags = 0;
                                    p->zOfs = 16.0f;
                                    p->v[0] = d3fl_WaterPoint(t, x, y, p->region);
                                    p->v[1] = d3fl_WaterPoint(t, x + 1, y + 1, p->region);
                                    p->v[2] = d3fl_WaterPoint(t, x, y + 1, p->region);
                                    p->uv = d3fl_UV[0];
                                    p++;
                                    p->tex = 0;
                                    p->vOfs = 0;
                                    p->uOfs = 0;
                                    p->flags = 0;
                                    p->region = cmask[k];
                                    p->zOfs = 16.0f;
                                    p->v[0] = d3fl_WaterPoint(t, x, y, p->region);
                                    p->v[1] = d3fl_WaterPoint(t, x + 1, y, p->region);
                                    p->v[2] = d3fl_WaterPoint(t, x + 1, y + 1, p->region);
                                    p->uv = d3fl_UV[1];
                                    p++;
                                }
                            }
                        r = t->wregions;
                        left = map->n * map->n / 2 * sizeof(D3FLWREG);
                        off = sizeof(D3FLWREG);
                        while (r->y != -1)
                            if (r->flag == -1) {
                                sprintf(msg, "PrepareFloorWater: removed region @ %i:[%i - %i] idx %i (hsize = %i)",
                                        r->y, r->x0, r->x1, r->idx, map->size);
                                memmove(r, r + 1, left - off);
                                left -= sizeof(D3FLWREG);
                            } else {
                                r++;
                                off += sizeof(D3FLWREG);
                            }
                    }
            mem_Free(cmask);
        }
        mem_Free(mask);
    }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            if (!map->tiles[j][i].water) {
                if (map->tiles[j][i].wregions) {
                    mem_Free(map->tiles[j][i].wregions);
                    map->tiles[j][i].wregions = 0;
                }
                if (map->tiles[j][i].wpoints) {
                    mem_Free(map->tiles[j][i].wpoints);
                    map->tiles[j][i].wpoints = 0;
                }
                if (map->tiles[j][i].wpolys) {
                    mem_Free(map->tiles[j][i].wpolys);
                    map->tiles[j][i].wpolys = 0;
                }
            }
    if (map->watertype == 0xff) {
        if (map->regions) {
            for (i = 0; i < map->nregions; i++)
                tex_Release((D3TEXHANDLE *)map->regions[i].tex);
            mem_Free(map->regions);
            map->regions = 0;
        }
        map->nregions = 0;
        if (map->water) {
            mem_Free(map->water);
            map->water = 0;
        }
    }
    map->dirty = 1;
}
