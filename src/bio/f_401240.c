/* 0x401240, writes a 4-byte value */

void bio_wr_i(VFSFILE *f, int i)
{
    vfs_fwrite(&i, 4, 1, f);
}
