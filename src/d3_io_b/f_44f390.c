/* 0x44f390 */

void bgi_MatOu(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).ou);
}
