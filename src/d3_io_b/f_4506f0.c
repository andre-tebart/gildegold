/* 0x4506f0, 0 when the file's header flags have bit 1 set (format 2 only), else 1 */

char d3_Unk4506f0(char *path)
{
    VFSFILE *f;
    int hdr, n;
    char c;
    char ret = 1;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fread(&hdr, 4, 1, f);
        bio_rd_c(f, &c);
        bio_rd_i(f, &n);
        bio_rd_c(f, &c);
        bio_rd_i(f, &n);
        bio_rd_c(f, &c);
        ret = 1;
        if (c == 2) {
            bio_rd_c(f, &c);
            if ((c & 2) == 2)
                ret = 0;
        }
        vfs_fclose(f);
    }
    return ret;
}
