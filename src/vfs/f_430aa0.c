/* 0x430aa0, returns the size of a written memory file. */

int vfs_fclose(VFSFILE *f)
{
    int ret = 0;
    unsigned char *p;
    uLong x;
    int i;

    if (f) {
        if (f->flags.memory) {
            if (f->flags.compressed) {
                if (f->flags.read)
                    inflateEnd(&f->zs);
                else {
                    deflate(&f->zs, Z_FINISH);
                    deflateEnd(&f->zs);
                    if (f->zs.avail_out >= 8) {
                        p = f->zs.next_out;
                        x = f->crc;
                        for (i = 0; i < 4; i++) {
                            *p = (unsigned char)x;
                            x >>= 8;
                            p++;
                        }
                        x = f->zs.total_in;
                        for (i = 0; i < 4; i++) {
                            *p = (unsigned char)x;
                            x >>= 8;
                            p++;
                        }
                        f->zs.total_out += 8;
                        f->zs.next_out += 8;
                        f->zs.avail_out -= 8;
                    }
                }
            }
            if (!f->flags.read)
                ret = f->zs.total_out;
        } else if (f->fp) {
            if (f->flags.read && f->flags.inzip) {
                unzCloseCurrentFile(f->zip);
                unzClose(f->zip);
            } else if (f->flags.compressed)
                gzclose(f->gz);
            else
                fclose(f->fp);
        }
        vfs_OpenCount--;
        pool_Free(&vfs_FilePool, f);
    }
    return ret;
}
