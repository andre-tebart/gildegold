/* 0x4011f0, writes one byte */

void bio_wr_c(VFSFILE *f, char c)
{
    vfs_fwrite(&c, 1, 1, f);
}
