/* 0x431170 */

long vfs_ftell(VFSFILE *f)
{
    long r = -1;

    if (f && (f->fp || f->flags.memory)) {
        if (f->flags.memory)
            r = f->zs.total_in;
        else if (f->flags.read) {
            if (f->flags.inzip)
                r = unztell(f->zip);
            else if (f->flags.compressed)
                r = gztell(f->gz);
            else
                r = ftell(f->fp);
        } else if (f->flags.compressed)
            r = gztell(f->gz);
        else
            r = ftell(f->fp);
    }
    return r;
}
