/* 0x4507a0, opens a chunk file and skips to the chunk with id 0xfab50005; 0 if there is none */

VFSFILE *d3_Unk4507a0(char *path)
{
    VFSFILE *f;
    int hdr, len, id, tag;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fread(&hdr, 4, 1, f);
        while (1) {
            tag = bgi_ReadId(f);
            if (tag == 0x2d) {
                bio_rd_i(f, &len);
                bio_rd_i(f, &id);
                if (id == 0xfab50005)
                    return f;
                vfs_fseek(f, len - 4, 1);
            } else if (tag == 0x2b)
                break;
            else {
                bio_rd_i(f, &len);
                vfs_fseek(f, len, 1);
            }
        }
        vfs_fclose(f);
    }
    return 0;
}
