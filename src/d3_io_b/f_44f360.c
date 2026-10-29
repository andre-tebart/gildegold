/* 0x44f360 */

void bgi_MatSv(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).sv);
}
