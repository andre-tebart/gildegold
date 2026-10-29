/* 0x41fc00 @deep behaviour: the transcription follows the Ghidra reading of 0x41fc00 (same channel shift counts, y0/y1 and x0/x1 bounds, p00/p01/p10/p11 with the down row and right column guards, the same accumulators and the OR of hb/dx + lb << bs etc.; the original's lPitch >> 1 down row is kept as the binary has it, as in the other scalers); the 703 code hunks are the frame (ours 0x8c against 0xb4) and register/scheduling shape; readability met */

void surf_Up32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    unsigned int m;
    unsigned int y, x, y0, y1, x0, x1, dy, dx, yy, xx, down, right;
    unsigned int p00, p01, p10, p11;
    unsigned int r00, g00, b00, r01, g01, b01;
    unsigned int al, ag, ab, ar2, ag2, ab2, hr, hg, hb;
    int lr, lg, lb;
    unsigned int *srow, *d;

    rs = 0;
    for (m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (; m; m >>= 1)
        ;
    gs = 0;
    for (m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (; m; m >>= 1)
        ;
    bs = 0;
    for (m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (; m; m >>= 1)
        ;
    y0 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        srow = (unsigned int *)((char *)src->lpSurface + src->lPitch * y);
        x0 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            right = x + 1 < src->dwWidth;
            p00 = srow[x];
            r00 = (p00 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g00 = (p00 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b00 = (p00 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p01 = srow[right + x];
            r01 = (p01 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g01 = (p01 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b01 = (p01 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p10 = srow[down + x];
            p11 = srow[down + right + x];
            if (dy) {
                al = ag = ab = ar2 = ag2 = ab2 = 0;
                yy = 0;
                do {
                    lr = al / dy + r00;
                    lg = ag / dy + g00;
                    lb = ab / dy + b00;
                    if (dx) {
                        hr = hg = hb = 0;
                        d = (unsigned int *)((char *)dst->lpSurface + (y0 + yy) * dst->lPitch) + x0;
                        xx = dx;
                        do {
                            *d++ = hb / dx + lb << bs | hg / dx + lg << gs | hr / dx + lr << rs;
                            hb += ab2 / dy + b01 - lb;
                            hg += ag2 / dy + g01 - lg;
                            hr += ar2 / dy + r01 - lr;
                        } while (--xx);
                    }
                    al += ((p10 & src->ddpfPixelFormat.dwRBitMask) >> rs) - r00;
                    ag += ((p10 & src->ddpfPixelFormat.dwGBitMask) >> gs) - g00;
                    ab += ((p10 & src->ddpfPixelFormat.dwBBitMask) >> bs) - b00;
                    ar2 += ((p11 & src->ddpfPixelFormat.dwRBitMask) >> rs) - r01;
                    ag2 += ((p11 & src->ddpfPixelFormat.dwGBitMask) >> gs) - g01;
                    ab2 += ((p11 & src->ddpfPixelFormat.dwBBitMask) >> bs) - b01;
                } while (++yy < dy);
            }
            x0 = x1;
        }
        y0 = y1;
    }
}
