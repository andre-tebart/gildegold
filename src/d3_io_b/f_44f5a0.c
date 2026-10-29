/* 0x44f5a0, the material of a polygon (a byte, or an int with more than 254 materials) */

void bgi_PolyMat(void *f, BGIRD *c)
{
    unsigned char b;

    BGI_P(c).tex = -1;
    if (c->nmats > 0xfe) {
        bio_rd_i(f, &BGI_P(c).mat);
        c->polyidx++;
        return;
    }
    bio_rd_c(f, (char *)&b);
    if (b == 0xff) {
        BGI_P(c).mat = -1;
        c->polyidx++;
        return;
    }
    BGI_P(c).mat = b;
    c->polyidx++;
}
