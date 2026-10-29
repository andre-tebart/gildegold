/* 0x44f500, texture coordinates of a polygon: three u, three v, then three values that are skipped */

void bgi_PolyUV(void *f, BGIRD *c)
{
    int i, t;

    for (i = 0; i < 3; i++)
        bio_rd_i(f, (int *)&BGI_P(c).uv[i][0]);
    for (i = 0; i < 3; i++)
        bio_rd_i(f, (int *)&BGI_P(c).uv[i][1]);
    for (i = 0; i < 3; i++)
        bio_rd_i(f, &t);
}
