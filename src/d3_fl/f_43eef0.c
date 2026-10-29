/* 0x43eef0, draws the tiles of r into the 16-bit texture pixels dst */

void d3fl_Render(unsigned short *dst, D3FLTILES *r)
{
    unsigned char *s[4];
    unsigned long *w;
    unsigned int m, i, j;
    unsigned char **base;
    int off;

    s[0] = (unsigned char *)d3fl_W16;
    s[1] = (unsigned char *)d3fl_W32;
    s[2] = (unsigned char *)d3fl_W64;
    d3fl_TileN = r->texsize / r->n;
    m = min(64, max(16, d3fl_TileN));
    w = (unsigned long *)s[m >> 5];
    base = r->src[2 - (m >> 5)];
    d3fl_SrcStep = r->n * m * 4 / r->texsize;
    d3fl_WPitch = d3fl_SrcStep * m;
    d3fl_SrcPitch = d3fl_SrcStep << 6;
    d3fl_DstPitch = r->texsize * 2;
    for (j = 0; j < r->n; j++)
        for (i = 0; i < r->n; i++) {
            off = ((((r->y + j) * m) & 0x3f) * 64 + (((r->x + i) * m) & 0x3f)) * 4;
            s[0] = base[r->tiles[j][i]] + off;
            s[1] = base[r->tiles[j][i + 1]] + off;
            s[2] = base[r->tiles[j + 1][i]] + off;
            s[3] = base[r->tiles[j + 1][i + 1]] + off;
            d3fl_Blend(dst + (r->texsize * j + i) * d3fl_TileN, w, s, d3fl_TileN);
        }
    if (dd_HasMMX)
        _m_empty();
}
