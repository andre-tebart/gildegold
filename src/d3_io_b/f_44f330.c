/* 0x44f330 */

void bgi_MatSu(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).su);
}
