/* 0x420e70, maps the image to palette indices: without dither through a lookup table over the 5-bit
   colors (qnt_Nearest per entry), with dither by error diffusion (err0 / err1 rows, cache)
   @deep behaviour: register / addressing / reload spellings only - grade.py code 147, regs 14; unitdiff 318 B / 87 asm lines (2026-10-26) (the clamp loop's three stores run on ebp/esi/edi with the 0x200 folded into one base instead of three, [eax+ecx+0xc] vs [ecx+eax+0xc], the ecx/edi/eax assignments of the palette loops and the cur / prev / lim slot reloads); same stores, same values, no meaning error; the clamp / palette spelling space (216+ variants) is in notes/units/qnt.md */

int qnt_Map(unsigned char *rgb, unsigned char *out, int w, int h, int dither)
{
    int e2;
    int cnt;
    unsigned char *lut;
    unsigned int i, n, off;
    unsigned char *clamp, *cl;
    short *cache, *err0, *err1, *cur, *prev;
    signed char *lim;
    int flip, y, x, dir, k, r, g, b, er, eg, eb, c;

    if (!dither) {
        n = w * h;
        lut = malloc(0x8000);
        if (lut) {
            for (i = 0, off = 0; off < 0x8000 * sizeof(QNODE); off += sizeof(QNODE), i++)
                if (((QNODE *)(off + (char *)qnt_Level[5]))->count)
                    lut[i] = qnt_Nearest((unsigned char)(i >> 7) & 0x80 | (unsigned char)((i & 4) << 1)
                                             | (unsigned char)(i >> 1) & 0x10 | (unsigned char)(i >> 3) & 0x20
                                             | (unsigned char)(i >> 5) & 0x40,
                                         (unsigned char)(i >> 6) & 0x80 | (unsigned char)(i >> 4) & 0x40
                                             | (unsigned char)(i >> 2) & 0x20 | (unsigned char)((i & 2) << 2)
                                             | (unsigned char)i & 0x10,
                                         (unsigned char)(((i & 1) << 2 | i & 8) << 1) | (unsigned char)(i >> 1) & 0x20
                                             | (unsigned char)(i >> 3) & 0x40 | (unsigned char)(i >> 5) & 0x80);
            for (i = 0; i < n; i++) {
                out[i] = lut[qnt_R[rgb[0]] + qnt_B[rgb[2]] + qnt_G[rgb[1]]];
                rgb += 3;
            }
            free(lut);
            return 1;
        }
    } else {
        flip = 0;
        cnt = w * 3 + 6;
        clamp = malloc(0x300);
        cl = clamp + 0x100;
        cache = malloc(0x10000);
        err0 = malloc(cnt * sizeof(short));
        err1 = malloc(cnt * sizeof(short));
        lim = malloc(0x200);
        if (clamp) {
            if (cache && err0 && err1 && lim) {
                for (k = 0; k < cnt; k++)
                    err0[k] = 0;
                memset(cache, 0xff, 0x10000);
                /* cl[k - 256] is clamp[k]: this spelling and order of the three stores give VC6's code */
                for (k = 0; k < 256; k++) {
                    cl[k] = k;
                    clamp[k + 512] = 255;
                    cl[k - 256] = 0;
                }
                for (k = 0; k < 256; k++) {
                    lim[k] = -20;
                    lim[k + 256] = 20;
                }
                for (k = -20; k <= 20; k++)
                    lim[k + 256] = k;
                for (y = 0; y < h; y++) {
                    if (flip) {
                        dir = -1;
                        rgb += w * 3 - 3;
                        out += w - 1;
                        cur = err0 + w * 3;
                        prev = err1 + 3;
                    } else {
                        dir = 1;
                        cur = err1 + w * 3;
                        prev = err0 + 3;
                    }
                    cur[2] = 0;
                    cur[1] = 0;
                    cur[0] = 0;
                    for (x = w; x > 0; x--) {
                        r = cl[rgb[0] + (prev[0] + 8 >> 4)];
                        g = cl[rgb[1] + (prev[1] + 8 >> 4)];
                        b = cl[rgb[2] + (prev[2] + 8 >> 4)];
                        k = ((r & 0xf8) << 5 | g & 0xf8) << 2 | b >> 3;
                        if (cache[k] < 0)
                            cache[k] = qnt_Nearest(r, g, b);
                        prev += 3;
                        cur -= 3;
                        *out = (unsigned char)cache[k];
                        c = cache[k];
                        er = lim[r - qnt_PalR[c] + 256];
                        eg = lim[g - qnt_PalG[c] + 256];
                        eb = lim[b - qnt_PalB[c] + 256];
                        cur[0] = er;
                        /* error diffusion: the error goes to the neighbours with weights 3, 5, 7 (of 16) through the
                           running sums er + 2er, + 2er, + 2er (VC6 builds the multiples as this add chain; the
                           products er * 3 / 5 / 7 compile differently) */
                        e2 = er + er;
                        er += e2;
                        cur[6] += er;
                        er += e2;
                        cur[3] += er;
                        e2 += er;
                        prev[0] += e2;
                        cur[1] = eg;
                        e2 = eg + eg;
                        eg += e2;
                        cur[7] += eg;
                        eg += e2;
                        cur[4] += eg;
                        e2 += eg;
                        prev[1] += e2;
                        cur[2] = eb;
                        e2 = eb + eb;
                        eb += e2;
                        cur[8] += eb;
                        eb += e2;
                        cur[5] += eb;
                        e2 += eb;
                        prev[2] += e2;
                        rgb += dir * 3;
                        out += dir;
                    }
                    if (y % 2 == 1) {
                        rgb += (w + 1) * 3;
                        out += w + 1;
                    }
                    flip = !flip;
                }
                free(clamp);
                free(cache);
                free(err0);
                free(err1);
                free(lim);
                return 1;
            }
            free(clamp);
        }
        if (cache)
            free(cache);
        if (err0)
            free(err0);
        if (err1)
            free(err1);
        if (lim)
            free(lim);
    }
    return 0;
}
