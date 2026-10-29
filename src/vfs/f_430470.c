/* 0x430470, opens a file of the directory tree. mode: R read (default), W write, B binary (default), T text,
   Z gzip (default), N plain. Files inside zip archives can only be read, and only in binary mode.
   @deep behaviour: 483 code hunk lines and 36 slot lines are the frame size (ours 0x364, original 0x368 - one 4-byte local/temp the original keeps crossing the calls) and the resulting shift of every stack reference (+4) plus the block order of the three pool_Alloc / unzGetCurrentFileInfo sites; the mode-parsing switch, all compares and every non-stack immediate (0x14, 0xef, 0xdf, 0xde, 8, 3, 0x140, 0x10) appear with equal counts on both sides, the differing register spellings (eax/edi, al/cl) are the same operations - no statement, constant or callee differs (full asmdiff mnemonic histogram; hunks at instructions 0/50/69/148/185/200 read). Frame/slot allocation - deep */

VFSFILE *vfs_Tfopen(char *name, char *mode, int unk)
{
    VFSFILE *f = 0;
    VFSDIR *r;
    char *modes[2];
    char tmp[0x100];
    unz_file_info info;
    char path[0x100];
    char buf[0x100];
    unsigned char binary, z, read;
    VFSENTRY *e;
    unzFile zip;
    void *fp;
    char *p;
    int err;

    if (!name || !*name || !mode || !unk)
        return 0;
    read = 1;
    binary = 1;
    z = 1;
    for (p = mode; *p; p++) {
        switch (toupper(*p)) {
        case 'B':
            binary = 1;
            break;
        case 'N':
            z = 0;
            break;
        case 'R':
            read = 1;
            break;
        case 'T':
            binary = 0;
            break;
        case 'W':
            read = 0;
            break;
        case 'Z':
            z = 1;
            break;
        }
    }
    if (read) {
        e = vfs_FindFile(name, (VFSDIR *)unk, &r);
        if (!e)
            goto notfound;
        if (e->zip) {
            if (!binary) {
                err_Message("Unsupported Read-Mode in vfs:Open (textfile inside zip)");
                return 0;
            }
            vfs_EntryPath(0, e->zip, 0, path);
            strcat(path, vfs_Ext[e->ext]);
            vfs_ToBackslashes(path);
            zip = unzOpen(path);
            if (!zip)
                goto notfound;
            vfs_EntryPath(e, r, e->zip, buf);
            _strupr(buf);
            if (e->pos > 0) {
                err = unzGoToFilePos(zip, e->pos, e->num);
                if (unzGetCurrentFileInfo(zip, &info, tmp, 0x100, 0, 0, 0, 0) == 0) {
                    _strupr(tmp);
                    vfs_ToSlashes(tmp);
                    if (info.uncompressed_size > 0 && !strcmp(buf, tmp))
                        err = unzOpenCurrentFile(zip);
                    if (!err) {
                        f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
                        f->zip = zip;
                        f->flags.checkgz = 1;
                        f->flags.inzip = 1;
                        goto opened;
                    }
                }
            } else {
                do {
                    if (unzGetCurrentFileInfo(zip, &info, tmp, 0x100, 0, 0, 0, 0) == 0) {
                        _strupr(tmp);
                        vfs_ToSlashes(tmp);
                        if (info.uncompressed_size > 0 && !strcmp(buf, tmp)) {
                            err = unzOpenCurrentFile(zip);
                            if (!err) {
                                f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
                                f->zip = zip;
                                f->flags.checkgz = 1;
                                f->flags.inzip = 1;
                                goto opened;
                            }
                            break;
                        }
                    }
                } while (unzGoToNextFile(zip) == 0);
            }
            unzClose(zip);
            goto notfound;
        }
        modes[0] = "rt";
        modes[1] = "rb";
        vfs_EntryPath(e, r, 0, path);
        vfs_ToBackslashes(path);
        if (z)
            fp = gzopen(path, modes[binary]);
        else
            fp = fopen(path, modes[binary]);
        if (!fp)
            goto notfound;
        f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
        f->fp = fp;
        f->flags.inzip = 0;
opened:
        strcpy(f->name, name);
        f->flags.memory = 0;
        f->flags.read = 1;
        f->flags.binary = binary;
        f->flags.compressed = z;
        vfs_OpenCount++;
        return f;
notfound:
        sprintf(buf, "vfs_Tfopen: Could not find \"%s\"!!", name);
        return f;
    }
    for (p = name; *p && (*p == '\\' || *p == '/'); p++)
        ;
    strcpy(tmp, p);
    vfs_ToSlashes(tmp);
    strcpy(buf, tmp);
    vfs_UpperCase(tmp);
    e = vfs_FindFile(tmp, (VFSDIR *)unk, &r);
    if (!e || e->zip) {
        if (!vfs_MakeDirs(buf, unk))
            return 0;
        if (!e) {
            vfs_AddFilePath(tmp, (VFSDIR *)unk, 0, 0, 0, 0, -1);
            e = vfs_FindFile(tmp, (VFSDIR *)unk, &r);
        }
    }
    if (!e) {
        sprintf(buf, "vfs_Tfopen: Could write to \"%s\"!!", name);
        err_Message(buf);
        return f;
    }
    modes[0] = "wt";
    modes[1] = "wb";
    e->date = -1;
    e->zip = 0;
    e->pos = 0;
    vfs_EntryPath(e, r, 0, path);
    vfs_ToBackslashes(path);
    if (z)
        fp = gzopen(path, modes[binary]);
    else
        fp = fopen(path, modes[binary]);
    if (!fp)
        return f;
    f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
    f->fp = fp;
    strcpy(f->name, name);
    f->flags.memory = 0;
    f->flags.read = 0;
    f->flags.binary = binary;
    f->flags.compressed = z;
    vfs_OpenCount++;
    return f;
}
