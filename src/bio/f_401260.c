/* 0x401260, reads three 4-byte values (a vector, as raw bits) */

void bio_rd_v3(VFSFILE *f, int *v)
{
    bio_rd_i(f, v);
    bio_rd_i(f, v + 1);
    bio_rd_i(f, v + 2);
}
