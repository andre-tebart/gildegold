/* 0x44f3f0 */

void bgi_MatRot(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).rot);
}
