/* 0x430e30 */

unsigned vfs_fwrite(void *buf, int size, int n, VFSFILE *f)
{
    unsigned int len;
    void *fp;
    char mem;
    int ret = -1;

    if (f && !f->flags.read) {
        fp = f->fp;
        if (fp || f->flags.memory) {
            len = size * n;
            mem = f->flags.memory;
            if (mem && !f->flags.compressed && f->zs.avail_out < len)
                len = f->zs.avail_out;
            if (len == 0)
                ret = 0;
            else if (buf) {
                if (mem) {
                    if (f->flags.compressed) {
                        f->zs.avail_in = len;
                        f->zs.next_in = buf;
                        deflate(&f->zs, Z_NO_FLUSH);
                        f->crc = crc32(f->crc, buf, len);
                        ret = len - f->zs.avail_in;
                    } else {
                    n = (int)f->zs.next_out;
                    VFS_COPY(n, buf, len);
                    f->zs.next_out += len;
                    f->zs.avail_out -= len;
                    f->zs.total_out += len;
                    ret = len;
                    }
                } else if (f->flags.compressed)
                    ret = gzwrite(fp, buf, len);
                else
                    ret = fwrite(buf, size, n, fp);
            }
        }
    }
    return ret;
}
