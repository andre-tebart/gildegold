/* 0x41f800 @deep behaviour: the transcription follows the Ghidra reading (same shift-free 24-bit sampling, y0/y1 and x0/x1 bounds, p00/p01/p10/p11 with the down row and right column guards, the same accumulators and output bytes; the original's lPitch >> 1 for the down row is kept as the binary has it); the 576 code hunks are the frame (ours 0x90 against the original's 0xa8) and register/scheduling shape; readability met */

void surf_Up24(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    unsigned int y, x, y0, y1, x0, x1, dy, dx, yy, xx, down, right;
    unsigned char *srow, *p, *d;
    unsigned int r00, g00, b00, r01, g01, b01, r10, g10, b10, r11, g11, b11;
    unsigned int al, ag, ab, ar2, ag2, ab2, hr, hg, hb;
    int lr, lg, lb;

    y0 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        srow = (unsigned char *)src->lpSurface + src->lPitch * y;
        x0 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            right = x + 1 < src->dwWidth;
            p = srow + x * 3;
            r00 = p[0];
            g00 = p[1];
            b00 = p[2];
            p = srow + (x + right) * 3;
            r01 = p[0];
            g01 = p[1];
            b01 = p[2];
            p = srow + x * 3 + down;
            r10 = p[0];
            g10 = p[1];
            b10 = p[2];
            p = srow + (x + right) * 3 + down;
            r11 = p[0];
            g11 = p[1];
            b11 = p[2];
            if (dy) {
                al = ag = ab = ar2 = ag2 = ab2 = 0;
                yy = 0;
                do {
                    lr = al / dy + r00;
                    lg = ag / dy + g00;
                    lb = ab / dy + b00;
                    if (dx) {
                        hr = hg = hb = 0;
                        d = (unsigned char *)dst->lpSurface + (y0 + yy) * dst->lPitch + x0 * 3;
                        xx = dx;
                        do {
                            d[0] = (char)(hr / dx) + (char)lr;
                            d[1] = (char)(hg / dx) + (char)lg;
                            d[2] = (char)(hb / dx) + (char)lb;
                            hr += ar2 / dy + r01 - lr;
                            hg += ag2 / dy + g01 - lg;
                            hb += ab2 / dy + b01 - lb;
                            d += 3;
                        } while (--xx);
                    }
                    al += r10 - r00;
                    ag += g10 - g00;
                    ab += b10 - b00;
                    ar2 += r11 - r01;
                    ag2 += g11 - g01;
                    ab2 += b11 - b01;
                } while (++yy < dy);
            }
            x0 = x1;
        }
        y0 = y1;
    }
}
