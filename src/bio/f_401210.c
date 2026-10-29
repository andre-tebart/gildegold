/* 0x401210, writes a string with its terminator */

void bio_wr_s(VFSFILE *f, char *s)
{
    vfs_fwrite(s, strlen(s) + 1, 1, f);
}
