/* 0x4011d0, reads a 4-byte value */

void bio_rd_i(VFSFILE *f, int *i)
{
    vfs_fread(i, 4, 1, f);
}
