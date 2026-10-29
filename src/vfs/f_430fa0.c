/* 0x430fa0, files in a zip can only be seeked forward (by reading), or reopened for a backward SEEK_SET. */

int vfs_fseek(VFSFILE *f, long offset, int origin)
{
    int ret = -1;
    long cur;
    void *p;

    if (f && f->fp && !f->flags.memory) {
        if (f->flags.read) {
            if (f->flags.inzip) {
                f->flags.checkgz = 0;
                if (origin == SEEK_SET) {
                    cur = unztell(f->zip);
                    if (cur <= offset) {
                        if (cur == offset)
                            ret = 0;
                        else {
                        offset -= cur;
                        p = mem_Alloc(offset, "vfs:fseek(inzip,SEEK_SET,skip)");
                        if (unzReadCurrentFile(f->zip, p, offset) == offset)
                            ret = 0;
                        mem_Free(p);
                        }
                    } else {
                    unzCloseCurrentFile(f->zip);
                    if (unzOpenCurrentFile(f->zip) == 0) {
                        p = mem_Alloc(offset, "vfs:fseek(inzip,SEEK_SET,skip)");
                        if (unzReadCurrentFile(f->zip, p, offset) == offset)
                            ret = 0;
                        mem_Free(p);
                    }
                    }
                } else if (origin == SEEK_CUR) {
                    if (offset >= 0) {
                        if (offset == 0)
                            ret = 0;
                        else {
                        p = mem_Alloc(offset, "vfs:fseek(inzip,SEEK_CUR,skip)");
                        if (unzReadCurrentFile(f->zip, p, offset) == offset)
                            ret = 0;
                        mem_Free(p);
                        }
                    } else
                    err_Message("Unsupported vfs_fseek(inzip,SEEK_CUR)");
                } else {
                    err_Message("Unsupported vfs_fseek(inzip,SEEK_END)");
                }
            } else if (f->flags.compressed)
                ret = gzseek(f->gz, offset, origin) == -1 ? -1 : 0;
            else
                ret = fseek(f->fp, offset, origin) ? -1 : 0;
        } else
            err_Message("Seeking only supported in read-only files");
    }
    return ret;
}
