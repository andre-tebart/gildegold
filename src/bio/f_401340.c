/* 0x401340, writes an array: element count, element size, then the data */

void bio_wr_array(VFSFILE *f, int size, int n, void *data)
{
    bio_wr_i(f, n);
    bio_wr_i(f, size);
    vfs_fwrite(data, size, n, f);
}
