/* 0x4012d0, writes three 4-byte values */

void bio_wr_v3(VFSFILE *f, int *v)
{
    bio_wr_i(f, v[0]);
    bio_wr_i(f, v[1]);
    bio_wr_i(f, v[2]);
}
