/* 0x44f1c0, flags packed in one byte */

void bgi_MatFlags1(void *f, BGIRD *c)
{
    unsigned char b;

    bio_rd_c(f, (char *)&b);
    BGI_M(c).c2 = b & 1;
    BGI_M(c).c5 = (b >> 1) & 1;
    BGI_M(c).c6 = (b >> 2) & 0xf;
    BGI_M(c).c3 = (b >> 6) & 1;
}
