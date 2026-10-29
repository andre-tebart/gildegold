/* 0x4463b0, map point under the world position pos (x/z) and, if h is given, the ground height there,
   interpolated on the triangle of the (lod: current detail level) mesh
   @deep B: read the whole 111-line asmdiff against 0x4463b0..0x446750 and diffclass: the first 131 instructions are equal, then the bilinear height samples are computed with ebp/edx/esi/edi in other roles (the flat indices and neighbours are the same expressions) and the four fild byte samples are scheduled in another order, 46 instructions equal again, and the second pick block repeats the same register swap; no different constant, callee, condition or field. Register allocation only, needs vargen/try.py */

char map_Pick(D3FLMAP *map, D3VECTOR *pos, char lod, int *px, int *py, float *h)
{
    D3VECTOR4 d;
    float u, v, h0, du, dv;
    int x, y, k, ix;
    unsigned int s, x1, y1;
    unsigned char *div;

    if (!map)
        return 0;
    d.x = pos->x - map->orgx;
    d.z = pos->z - map->orgz;
    if (map->scale != 0.0f)
        d.x /= map->scale;
    if (map->negScale != 0.0f)
        d.z /= map->negScale;
    *px = x = (int)(d.x + 0.5f);
    *py = (int)(d.z + 0.5f);
    if (*px < 0 || *px >= map->size || *py < 0 || *py >= map->size)
        return 0;
    if (h) {
        div = map->divide[0];
        s = 1;
        if (lod) {
            k = map->tiles[*py / map->n][x / map->n].lodLevel;
            if (k == 2 || k == 4) {
                s = k;
                div = map->divide[k >> 1];
            }
        }
        ix = x & ~(s - 1);
        y = *py & ~(s - 1);
        u = (d.x - ix) / s;
        v = (d.z - y) / s;
        if (div && (div[y / s * map->size / s + ix / s] & 0x80)) {
            if (u + v > 1.0f) {
                x1 = (ix + s) & map->mask;
                y1 = (y + s) & map->mask;
                h0 = map->height[y1 * map->size + x1];
                du = map->height[y1 * map->size + ix] - h0;
                dv = map->height[y * map->size + x1] - h0;
                u = 1.0f - u;
                v = 1.0f - v;
            } else {
                h0 = map->height[y * map->size + ix];
                du = map->height[y * map->size + ((ix + s) & map->mask)] - h0;
                dv = map->height[((y + s) & map->mask) * map->size + ix] - h0;
            }
        } else if (u + (1.0f - v) > 1.0f) {
            x1 = (ix + s) & map->mask;
            y1 = (y + s) & map->mask;
            h0 = map->height[y * map->size + x1];
            du = map->height[y * map->size + ix] - h0;
            dv = map->height[y1 * map->size + x1] - h0;
            u = 1.0f - u;
        } else {
            x1 = (ix + s) & map->mask;
            y1 = (y + s) & map->mask;
            h0 = map->height[y1 * map->size + ix];
            du = map->height[y1 * map->size + x1] - h0;
            dv = map->height[y * map->size + ix] - h0;
            v = 1.0f - v;
        }
        *h = (du * u + dv * v + h0) * map->hscale + map->orgy;
    }
    return 1;
}
