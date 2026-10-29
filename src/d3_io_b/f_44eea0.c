/* 0x44eea0, reads a chunk id: 0x2b at the end of the file, 0x27 for an invalid byte */

int bgi_ReadId(void *f)
{
    int id;
    char c;

    if (!vfs_fread(&c, 1, 1, f))
        return 0x2b;
    id = c;
    if (id < 0 || id >= 0x3b)
        id = 0x27;
    return id;
}
