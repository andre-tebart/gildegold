/* 0x446750, the visible tile vertex nearest to the screen position x, y within r pixels (weighted by depth);
   returns its map position as y << 16 | x and its camera-space position in out, or -1
   @deep B: read the whole 137-line asmdiff against 0x446750..0x446910 and diffclass: 12 equal instructions, then the ret/best and loop variables sit in other stack slots (0x10/0x14/0x18/0x1c/0x20/0x24 permuted; ret -1 through esi/0x18 in the original) and other registers (ebp vs eax/edx), with the fst/fld scheduling around the candidate block and the three nested counters in another order; no different constant, callee, condition or field. Register/slot allocation only, needs vargen/try.py */

int map_PickScreen(D3FLMAP *map, int x, int y, float r, D3VECTOR *out)
{
    D3PVERT *p;
    int tx, ty, s, nx, ny, n, i, k, j, ret;
    float r2, best, dx, dy, d, f;

    if (!map)
        return -1;
    r2 = r * r;
    best = 1.0f;
    ret = -1;
    for (ty = 0; ty < 8; ty++)
        for (tx = 0; tx < 8; tx++) {
            s = map->tiles[ty][tx].lodLevel;
            if (s > 0) {
                n = map->n / s + 1;
                if (tx == 7)
                    nx = n - 4 / s;
                else
                    nx = n;
                if (ty == 7)
                    ny = n - 4 / s;
                else
                    ny = n;
                n = ny * nx;
                p = (D3PVERT *)map->tiles[ty][tx].points;
                for (i = 0; i < n; i++, p++)
                    if (p->flags & 0x80) {
                        dx = x - p->sx;
                        dy = y - p->sy;
                        d = dx * dx + dy * dy;
                        if (d < r2) {
                            f = sqrt(d) / r * (p->z / d3_MaxZ);
                            if (f < best) {
                                k = i / nx * s + map->n * ty;
                                if (k < map->size) {
                                    j = map->n * tx + i % nx * s;
                                    if (j < map->size) {
                                        ret = k << 16 | j;
                                        out->x = p->x;
                                        out->y = p->y;
                                        out->z = p->z;
                                        best = f;
                                    }
                                }
                            }
                        }
                    }
            }
        }
    return ret;
}
