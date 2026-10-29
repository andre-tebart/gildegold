/* 0x44f160 */

void bgi_MatKind(void *f, BGIRD *c)
{
    bio_rd_c(f, (char *)&BGI_M(c).kind);
}
