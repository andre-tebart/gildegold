/* 0x44f7a0, two flag bits */

void bgi_Flags(void *f, BGIRD *c)
{
    unsigned char b;

    bio_rd_c(f, (char *)&b);
    c->flag0 = b & 1;
    c->flag1 = (b >> 1) & 1;
}
