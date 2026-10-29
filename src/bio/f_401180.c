/* 0x401180, reads one byte */

void bio_rd_c(VFSFILE *f, char *c)
{
    vfs_fread(c, 1, 1, f);
}
