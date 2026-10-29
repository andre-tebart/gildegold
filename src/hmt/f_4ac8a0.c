/* 0x4ac8a0, writes h to /gamedata/ai/historical.dfn (magic "SaFh", the counts, the records); 0 or -1 */

/* vfs_fwrite counts as unsigned here (size_t, fwrite): `> 0` gives the jbe of the original */
#define HMT_FW(b, s, n, f) ((unsigned int)vfs_fwrite(b, s, n, f) > 0)

int hmt_WriteBinaryFile(HMTFILE *h)
{
    int magic;
    VFSFILE *f;
    int i;

    magic = 0x68466153;
    f = vfs_Tfopen("/gamedata/ai/historical.dfn", "wb", (int)vfs_Root);
    if (f != 0) {
        if (HMT_FW(&magic, 4, 1, f) && HMT_FW(&h->np, 4, 1, f) && HMT_FW(&h->nk, 4, 1, f)) {
            for (i = 0; i < h->np; i++) {
                if (!HMT_FW(&h->p[i].idx, 4, 1, f) || !HMT_FW(&h->p[i].begin, 4, 1, f)
                    || !HMT_FW(&h->p[i].end, 4, 1, f) || !HMT_FW(h->p[i].vname, 0x10, 1, f)
                    || !HMT_FW(h->p[i].nname, 0x10, 1, f))
                    goto fail;
            }
            for (i = 0; i < h->nk; i++) {
                if (!HMT_FW(&h->k[i].idx, 4, 1, f) || !HMT_FW(&h->k[i].begin, 4, 1, f)
                    || !HMT_FW(&h->k[i].end, 4, 1, f) || !HMT_FW(h->k[i].vname, 0x10, 1, f)
                    || !HMT_FW(h->k[i].nname, 0x10, 1, f))
                    goto fail;
            }
            vfs_fclose(f);
            return 0;
        }
fail:
        vfs_fclose(f);
    }
    return -1;
}
