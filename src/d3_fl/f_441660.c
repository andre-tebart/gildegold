/* 0x441660, d3_ForEachObject callback of d3fl_MakeLight: finds the sun (type 7) and marks the map
   points lit by the other lights (bit 7 of the light map) */

typedef struct D3FLLIGHTCTX {
    D3FLMAP *map;
    D3OBJECT *light;
} D3FLLIGHTCTX;

char d3fl_LightCB(D3OBJECT *obj, void *ctx)
{
    D3FLMAP *map;
    D3VECTOR4 pos;
    float h, r, d, t, q;
    int x, y, n, n2, xx, yy, x0, x1, y0, y1, k;

    if (obj->type == 7) {
        if ((obj->objectFlags & 0x10) && !((D3FLLIGHTCTX *)ctx)->light) {
            ((D3FLLIGHTCTX *)ctx)->light = obj;
            return 1;
        }
    } else if (D3_L(obj)->power > 0.0 && D3_L(obj)->range > 0.0) {
        map = ((D3FLLIGHTCTX *)ctx)->map;
        r = D3_L(obj)->range;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
        if (map_Pick(map, (D3VECTOR *)&pos, 0, &x, &y, &h)) {
            d = pos.y - h;
            t = r * r - d * d;
            if (t > 0.0) {
                q = (float)sqrt(t) / (d3fl_LightMargin + map->scale);
                k = map->size * y + x;
                if (q > 1.0) {
                    n = (int)q;
                    n2 = (int)(q * q + 0.5f);
                    y1 = min(map->size - 1, y + n);
                    y0 = max(0, y - n);
                    for (yy = y0; yy <= y1; yy++) {
                        x1 = min(map->size - 1, x + n);
                        x0 = max(0, x - n);
                        for (xx = x0; xx <= x1; xx++)
                            if ((xx - x) * (xx - x) + (yy - y) * (yy - y) < n2)
                                map->light[map->size * yy + xx] |= 0x80;
                    }
                } else
                    map->light[k] |= 0x80;
            }
        }
    }
    return 1;
}
