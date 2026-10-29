/* 0x44f3c0 */

void bgi_MatOv(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).ov);
}
