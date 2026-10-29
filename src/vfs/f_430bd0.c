/* 0x430bd0, reads size * n bytes of an open VFS file into buf: from the memory image, from inside a zip
   archive (unzReadCurrentFile), inflated (gzread) or plain (fread); returns the number of bytes read (0 at
   the end of a memory image), (unsigned)-1 for a closed or write-only file */

int vfs_fread(void *buf, int size, int n, VFSFILE *f)
{
    char msg[0x100];
    unsigned int ret = -1;
    unsigned int len;
    z_stream *zs;
    void *fp;
    char mem;

    if (f && f->flags.read) {
        fp = f->fp;
        zs = &f->zs;
        if (fp || f->flags.memory) {
            len = size * n;
            mem = f->flags.memory;
            if (mem && !f->flags.compressed && f->zs.avail_in < len)
                len = f->zs.avail_in;
            if (len == 0)
                ret = len;
            else if (buf) {
                if (mem) {
                    if (f->flags.compressed) {
                        f->zs.next_out = buf;
                        f->zs.avail_out = len;
                        inflate(zs, Z_SYNC_FLUSH);
                        ret = len - f->zs.avail_out;
                    } else {
                    n = (int)fp;
                    VFS_COPY(buf, n, len);
                    ret = len;
                    zs->next_in += len;
                    f->zs.avail_in -= len;
                    f->zs.total_in += len;
                    }
                } else if (f->flags.inzip) {
                    ret = unzReadCurrentFile(fp, buf, len);
                    if (f->flags.checkgz && ret >= 2) {
                        f->flags.checkgz = 0;
                        if (((unsigned char *)buf)[0] == gz_magic[0] && ((unsigned char *)buf)[1] == gz_magic[1]) {
                            sprintf(msg, "vfs: gz-compressed file (\"%s\") in zip found!", f);
                        }
                    }
                } else if (f->flags.compressed)
                    ret = gzread(fp, buf, len);
                else
                    ret = fread(buf, size, n, fp);
            }
        }
    }
    return ret;
}
