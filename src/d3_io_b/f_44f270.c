/* 0x44f270 */

void bgi_MatFlags2(void *f, BGIRD *c)
{
    char b;

    bio_rd_c(f, &b);
    BGI_M(c).pad4 = b & 1;
    BGI_M(c).c7 = b & 0xfe;
}
