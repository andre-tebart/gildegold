/* 0x401300, writes four 4-byte values */

void bio_wr_v4(VFSFILE *f, int *v)
{
    bio_wr_i(f, v[0]);
    bio_wr_i(f, v[1]);
    bio_wr_i(f, v[2]);
    bio_wr_i(f, v[3]);
}
