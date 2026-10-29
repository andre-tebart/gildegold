/* 0x44d1f0, reads the texture set (.TXS) of a mesh: w x h names of 0x40 bytes */

void d3iot_LoadTexSet(char *name, char **out, int *w, int *h)
{
    char *path;
    void *f;
    int i, j;

    if (name) {
        path = d3io_StockFile(name, ".TXS");
        if (path) {
            f = vfs_Tfopen(path, "rb", vfs_Root);
            if (f) {
                bio_rd_i(f, &i);
                if (i == 0x23f209ae) {
                    bio_rd_i(f, &i);
                    bio_rd_i(f, &j);
                    if (i > 0 && j > 0) {
                        if (*out) {
                            mem_Free(*out);
                            *out = 0;
                        }
                        *out = mem_Alloc(j * i * 0x40, "d3_io:LoadtextureSet");
                        *w = i;
                        *h = j;
                        for (i = 0; i < *w; i++)
                            for (j = 0; j < *h; j++)
                                bio_rd_s(f, *out + (*h * i + j) * 0x40);
                    }
                }
                vfs_fclose(f);
            }
        }
    }
}
