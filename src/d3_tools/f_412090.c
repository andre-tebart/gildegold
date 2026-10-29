/* 0x412090, height range of the map under the triangle a, b, c (x/z in world units) */

int d3t_GetTriRange(D3TMAP *map, float *lo, float *hi, D3VECTOR *a, D3VECTOR *b, D3VECTOR *c)
{
    float sx, sz;
    int x[3], z[3];
    int t, y;
    int mn, mx, ret;
    float xl, xr, dl, dr;

    sx = 1.0f / map->cellx;
    x[0] = (int)((a->x - map->org.x) * sx);
    sz = 1.0f / map->cellz;
    z[0] = (int)((a->z - map->org.z) * sz);
    x[1] = (int)((b->x - map->org.x) * sx);
    z[1] = (int)((b->z - map->org.z) * sz);
    x[2] = (int)((c->x - map->org.x) * sx);
    z[2] = (int)((c->z - map->org.z) * sz);
    if (z[0] > z[1]) {
        t = x[0]; x[0] = x[1]; x[1] = t;
        t = z[0]; z[0] = z[1]; z[1] = t;
    }
    if (z[0] > z[2]) {
        t = x[0]; x[0] = x[2]; x[2] = t;
        t = z[0]; z[0] = z[2]; z[2] = t;
    }
    if (z[1] > z[2]) {
        t = x[1]; x[1] = x[2]; x[2] = t;
        t = z[1]; z[1] = z[2]; z[2] = t;
    }
    if (z[0] < map->size && z[2] >= 0 && (x[0] >= 0 || x[1] >= 0 || x[2] >= 0)
        && (x[0] < map->size || x[1] < map->size || x[2] < map->size)) {
        mn = 255;
        mx = 0;
        if (z[0] == z[1]) {
            if (z[0] == z[2]) {
                xl = min(min(x[0], x[1]), x[2]);
                xr = max(max(x[0], x[1]), x[2]);
                ret = d3t_GetRowRange(map, &mn, &mx, xl, xr, z[0]);
            } else {
                xl = x[0];
                xr = x[1];
                ret = d3t_GetRowRange(map, &mn, &mx, xl, xr, z[0]);
                dl = ((x[2] - x[0]) * 1.0) / (z[2] - z[0]);
                dr = ((x[2] - x[1]) * 1.0) / (z[2] - z[1]);
                xl += dl;
                xr += dr;
                for (y = z[0] + 1; y <= z[2]; y++) {
                    ret &= d3t_GetRowRange(map, &mn, &mx, xl, xr, y);
                    xl += dl;
                    xr += dr;
                }
            }
        } else {
            xl = x[0];
            dl = ((x[2] - x[0]) * 1.0) / (z[2] - z[0]);
            xr = x[0];
            dr = ((x[1] - x[0]) * 1.0) / (z[1] - z[0]);
            ret = 1;
            for (y = z[0]; y <= z[1]; y++) {
                ret &= d3t_GetRowRange(map, &mn, &mx, xl, xr, y);
                xl += dl;
                xr += dr;
            }
            dr = ((x[2] - x[1]) * 1.0) / (z[2] - z[1]);
            xr = x[1] - dr;
            for (; y <= z[2]; y++) {
                ret &= d3t_GetRowRange(map, &mn, &mx, xl, xr, y);
                xl += dl;
                xr += dr;
            }
        }
        if (!ret) {
            *lo = mn * map->scale + map->org.y;
            *hi = mx * map->scale + map->org.y;
        }
        return ret;
    }
    return 1;
}
