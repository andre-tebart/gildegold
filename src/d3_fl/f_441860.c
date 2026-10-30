/* 0x441860, the light map: bit 7 = lit by a point light (d3fl_LightCB), bits 0..6 = sun shade by the
   terrain normal; then bit 7 of the divide maps follows the light edges
   @deep B: code 357 / fpu 26 / slots 20 - the function was read in round 4 (notes/units/d3_fl.md: same size, frame 0x60 vs 0x68, the light part only differs in slot offsets / FPU operand order (the original loads map->negScale * hscale in the other order), the inlined d3fl_FtoI scheduling, and the divide-edge decision, where 288 + 144 group/order permutations were tried and none was closer than the kept five-group form (set {0111,1000}, clear {1101,1011,0100,0010,1001}, set {0001,1110,0110}, merged tails); no missing statement, wrong constant or field - needs vargen / listing */

void d3fl_MakeLight(D3FLMAP *map)
{
    D3FLLIGHTCTX ctx;
    D3VECTOR4 dir;
    D3VECTOR n;
    int size, x, y, s, dx, dz, iv, row;
    unsigned int l;
    unsigned char *h, *lp, *up, *down, *p, *drow;
    float kx, ky, kz, ky2, half, fx, fz, inv, d;
    int step[3];
    unsigned char a, b, c, e;

    if (!map->nolight && map->light) {
        d3fl_Invalidate(map);
        size = map->size;
        D3_ZERO(map->light, size * size);
        ctx.map = map;
        ctx.light = 0;
        d3_ForEachObject(d3_DefaultScene, 0, 0xe, d3fl_LightCB, &ctx);
        if (ctx.light) {
            half = D3_L(ctx.light)->power * 0.5f;
            map->ambient[0] = D3_L(ctx.light)->color.x * (1.0f / 255.0f);
            map->ambient[1] = D3_L(ctx.light)->color.y * (1.0f / 255.0f);
            map->ambient[2] = D3_L(ctx.light)->color.z * (1.0f / 255.0f);
            d3_DirToWorldD(ctx.light, &d3fl_Down, (D3VECTOR *)&dir);
            lp = map->light;
            h = map->height;
            kx = map->negScale * map->hscale * 2.0f;
            ky = map->negScale * map->scale * -4.0f;
            kz = map->hscale * map->scale * 2.0f;
            for (y = 0, row = 0; y < size; y++, row += size) {
                ky2 = ky * ky;
                up = h + ((y - 1) & (size - 1)) * size;
                down = h + ((y + 1) & (size - 1)) * size;
                for (x = 0; x < size; x++) {
                    dx = h[row + ((x + 1) & (size - 1))] - h[row + ((x - 1) & (size - 1))];
                    fx = dx * kx;
                    dz = down[x] - up[x];
                    fz = dz * kz;
                    inv = 1.0f / (float)sqrt(fx * fx + fz * fz + ky2);
                    n.x = inv * fx;
                    n.y = ky * inv;
                    n.z = inv * fz;
                    d = n.x * dir.x + n.y * dir.y + n.z * dir.z;
                    if (d < 0.0f) {
                        d3fl_FtoI(&iv, half * d3fl_Shade[(int)(d * -1023.0)]);
                        lp[row + x] |= min((unsigned int)iv, 0x7f);
                    }
                }
            }
        } else {
            map->ambient[0] = 255.0f;
            map->ambient[1] = 255.0f;
            map->ambient[2] = 255.0f;
        }
        step[0] = 1;
        step[1] = 2;
        step[2] = 4;
        for (l = 0; l < 3; l++) {
            s = step[l];
            size = map->size / s;
            drow = map->divide[l];
            for (y = 0; y < size - 1; y++, drow += size) {
                p = drow;
                for (x = 0; x < size - 1; x++, p++) {
                    int k = (map->size * y + x) * s;

                    a = map->light[k] >> 7;
                    b = map->light[k + s] >> 7;
                    c = map->light[k + map->size * s] >> 7;
                    e = map->light[k + (map->size + 1) * s] >> 7;
                    if ((!a && b && c && e) || (a && !b && !c && !e))
                        *p |= 0x80;
                    else if ((a && b && !c && e) || (a && !b && c && e) || (!a && b && !c && !e) || (!a && !b && c && !e) || (a && !b && !c && e))
                        *p &= 0x7f;
                    else if ((!a && !b && !c && e) || (a && b && c && !e) || (!a && b && c && !e))
                        *p |= 0x80;
                }
            }
        }
    }
}
