/* 0x4acae0, reads /gamedata/ai/historical.dfn into h (frees what h held); 0 or -1 (h emptied) */

#define HMT_FR(b, s, n, f) ((unsigned int)vfs_fread(b, s, n, f) > 0)

int hmt_ReadBinaryFile(HMTFILE *h)
{
    int magic;
    VFSFILE *f;
    int i;

    magic = 0x68466153;
    f = vfs_Tfopen("/gamedata/ai/historical.dfn", "rb", (int)vfs_Root);
    i = 0;
    if (f != 0) {
        if (HMT_FR(&magic, 4, 1, f) && magic == 0x68466153 && HMT_FR(&h->np, 4, 1, f)
            && HMT_FR(&h->nk, 4, 1, f) && h->np > 0 && h->nk > 0) {
            if (h->k != 0) {
                mem_Free(h->k);
                h->k = 0;
            }
            if (h->p != 0) {
                mem_Free(h->p);
                h->p = 0;
            }
            h->k = mem_Alloc(h->nk * sizeof(HMTREC), "hmt_CreateBinaryFile");
            if (h->k != 0) {
                h->p = mem_Alloc(h->np * sizeof(HMTREC), "hmt_CreateBinaryFile");
                if (h->p != 0) {
                    memset(h->k, 0, h->nk * sizeof(HMTREC));
                    memset(h->p, 0, h->np * sizeof(HMTREC));
                    for (i = 0; i < h->np; i++) {
                        if (!HMT_FR(&h->p[i].idx, 4, 1, f) || !HMT_FR(&h->p[i].begin, 4, 1, f)
                            || !HMT_FR(&h->p[i].end, 4, 1, f) || !HMT_FR(h->p[i].vname, 0x10, 1, f)
                            || !HMT_FR(h->p[i].nname, 0x10, 1, f))
                            goto fail;
                    }
                    for (i = 0; i < h->nk; i++) {
                        if (!HMT_FR(&h->k[i].idx, 4, 1, f) || !HMT_FR(&h->k[i].begin, 4, 1, f)
                            || !HMT_FR(&h->k[i].end, 4, 1, f) || !HMT_FR(h->k[i].vname, 0x10, 1, f)
                            || !HMT_FR(h->k[i].nname, 0x10, 1, f))
                            goto fail;
                    }
                    vfs_fclose(f);
                    return 0;
                }
            }
        }
fail:
        vfs_fclose(f);
    }
    if (h->k != 0) {
        mem_Free(h->k);
        h->k = 0;
    }
    if (h->p != 0) {
        mem_Free(h->p);
        h->p = 0;
    }
    h->np = 0;
    h->nk = 0;
    return -1;
}
