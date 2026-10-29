/* 0x43f080, bilinear weights of the four corners for an n x n tile, 4 bytes per pixel */

void d3fl_MakeWeights(unsigned char *p, unsigned int n)
{
    int step = 0x100 / n;
    unsigned int x, y;
    int w[4], d[4];

    for (y = 0; y < n; y++, p += n * 4) {
        w[2] = (y * step) << 8;
        w[0] = 0xffff - w[2];
        w[3] = 0;
        w[1] = 0;
        d[0] = -(d[1] = (w[0] >> 8) * step);
        d[2] = -(d[3] = (w[2] >> 8) * step);
        for (x = 0; x < n; x++) {
            p[x * 4] = w[0] >> 8;
            p[x * 4 + 1] = w[1] >> 8;
            p[x * 4 + 2] = w[2] >> 8;
            p[x * 4 + 3] = w[3] >> 8;
            w[0] += d[0];
            w[1] += d[1];
            w[2] += d[2];
            w[3] += d[3];
        }
    }
}
