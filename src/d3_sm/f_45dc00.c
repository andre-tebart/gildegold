/* 0x45dc00, samples the heights and ground types of the terrain into the map @deep behaviour: 544 code hunks, but all of them are the same operations with other register / stack-slot names or in another order (a full read of the asmdiff: 170 ours vs 181 original instructions, no statement missing); the instruction multiset differs only by 2 add / 2 xor / 4 mov / 2 lea, the same x*step+step masks and xor-zeroing strength-reduced differently, plus 5 fill nops; the C's loops (heights by __ftol, count[15] ground-type vote, step-halving midpoint fill) are the same as the Ghidra pseudocode, readability already readable; restoring the original's allocation is byte work */

void d3sm_Sample(D3TERRAIN *t, D3SM *h)
{
    unsigned int count[15];
    unsigned int n, ts, step, half, f, x, z, xx, zz, k, best, max, sum;
    unsigned int a, b, c, d;
    int avg;
    float inv;

    n = h->size;
    ts = t->size;
    step = n / ts;
    inv = 1.0f / h->cell.y;
    if (step == 0) {
        f = ts / n;
        for (z = 0; z < n; z++) {
            for (x = 0; x < n; x++) {
                sum = 0;
                for (zz = z * f; zz < z * f + f; zz++) {
                    for (xx = x * f; xx < x * f + f; xx++)
                        sum += (int)((t->heights[zz * ts + xx] * t->axisy.y + t->origin.y - h->origin.y) * inv);
                }
                h->data[z * n + x] = sum / (f * f);
                D3_ZERO(count, sizeof(count));
                for (zz = z * f; zz < z * f + f; zz++) {
                    for (xx = x * f; xx < x * f + f; xx++)
                        count[d3sm_GetGround(xx, zz, t)]++;
                }
                max = 0;
                best = -1;
                for (k = 0; k < 15; k++) {
                    if (count[k] > max) {
                        max = count[k];
                        best = k;
                    }
                }
                if (best != -1)
                    h->entries[z * n + x].type = best;
                else
                    h->entries[z * n + x].type = 0;
            }
        }
        return;
    }
    for (z = 0; z < ts; z++) {
        for (x = 0; x < ts; x++) {
            h->data[(z * n + x) * step] = (int)((t->heights[z * ts + x] * t->axisy.y + t->origin.y - h->origin.y) * inv);
            h->entries[(z * n + x) * step].type = d3sm_GetGround(x, z, t);
        }
    }
    for (; step > 1; step = half) {
        half = step >> 1;
        for (z = 0; z < n / step; z++) {
            for (x = 0; x < n / step; x++) {
                a = h->data[z * step * n + x * step];
                b = h->data[z * step * n + (x * step + step & n - 1)];
                c = h->data[(z * step + step & n - 1) * n + x * step];
                d = h->data[(z * step + step & n - 1) * n + (x * step + step & n - 1)];
                avg = (int)(a + b + c + d) / 4;
                h->data[z * step * n + x * step + half] = (avg + (a + b) * 2) / 5;
                h->data[(z * step + half) * n + x * step] = (avg + (a + c) * 2) / 5;
                h->data[(z * step + half) * n + x * step + half] = avg;
                h->entries[z * step * n + x * step + half].type = h->entries[z * step * n + x * step].type;
                h->entries[(z * step + half) * n + x * step].type = h->entries[z * step * n + x * step].type;
                h->entries[(z * step + half) * n + x * step + half].type = h->entries[z * step * n + x * step].type;
            }
        }
    }
}
