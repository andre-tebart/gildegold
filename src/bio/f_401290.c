/* 0x401290, reads four 4-byte values */

void bio_rd_v4(VFSFILE *f, int *v)
{
    bio_rd_i(f, v);
    bio_rd_i(f, v + 1);
    bio_rd_i(f, v + 2);
    bio_rd_i(f, v + 3);
}
