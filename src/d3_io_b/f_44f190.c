/* 0x44f190 */

void bgi_MatC1(void *f, BGIRD *c)
{
    bio_rd_c(f, (char *)&BGI_M(c).c1);
}
