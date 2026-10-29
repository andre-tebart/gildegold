/* 0x4311d0, opens a memory buffer as a file. mode: R read (default), W write, Z deflated, N not deflated;
   without Z or N a read buffer is deflated if it starts with the gzip magic.
   @deep behaviour: 39 code hunk lines are block layout and slots - the original merges the detect block's z=0 path with the else of the later if(z) (xor bl,bl; mov [esp+0x14],bl; the three f->zs store, then jmp over the deflated setup; the if(z) else jumps into the middle at 0x43129a), while ours lays the else out separately; the two branch polarities (size<=10: ours jbe to the else block, original ja over it; and jne/je at instruction 98) are the same tests, changed and unchanged statements and every constant (0xa, 0x1, gz_magic, 0x38, 0xf1) agree; local init order ([esp+0x14]/[esp+0x18]/[esp+0x13]) and 16 slot hunk lines are the frame. Tried and reverted: the if (size > 10 && magic) orientation (same layout). Block layout / slot allocation - deep */

VFSFILE *vfs_MemOpen(unsigned char *buf, unsigned int size, char *mode)
{
    VFSFILE *f;
    unsigned char z = 1;
    unsigned char read = 1;
    char detect = 1;
    char *p;

    if (!buf || size <= 0)
        return 0;
    for (p = mode; *p; p++) {
        switch (toupper(*p)) {
        case 'N':
            z = 0;
            detect = 0;
            break;
        case 'R':
            read = 1;
            break;
        case 'W':
            read = 0;
            break;
        case 'Z':
            z = 1;
            detect = 0;
            break;
        }
    }
    f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
    f->flags.checkgz = 0;
    f->flags.inzip = 0;
    f->flags.binary = 1;
    f->flags.memory = 1;
    if (read) {
        if (detect) {
            if (size <= 10 || buf[0] != gz_magic[0] || buf[1] != gz_magic[1])
                z = 0;
            else
                z = 1;
        }
        if (z) {
            f->zs.next_in = buf + 10;
            f->zs.avail_in = size - 10;
            f->zs.next_out = 0;
            f->zs.avail_out = 0;
            f->zs.zalloc = 0;
            f->zs.zfree = 0;
            if (inflateInit2(&f->zs, -MAX_WBITS) != Z_OK)
                goto fail;
        } else {
            f->zs.next_in = buf;
            f->zs.avail_in = size;
            f->zs.total_in = 0;
        }
    } else if (z) {
        if (size < 18)
            goto fail;
        buf[0] = gz_magic[0];
        buf[1] = gz_magic[1];
        buf[2] = Z_DEFLATED;
        buf[3] = buf[4] = buf[5] = buf[6] = buf[7] = buf[8] = 0;
        buf[9] = 0x0b;
        f->zs.next_in = 0;
        f->zs.avail_in = 0;
        f->zs.next_out = buf + 10;
        f->zs.avail_out = size - 10;
        f->zs.zalloc = 0;
        f->zs.zfree = 0;
        f->crc = crc32(0, 0, 0);
        if (deflateInit2(&f->zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
fail:
            pool_Free(&vfs_FilePool, f);
            return 0;
        }
        f->zs.total_out = 10;
    } else {
        f->zs.next_out = buf;
        f->zs.avail_out = size;
        f->zs.total_out = 0;
    }
    vfs_OpenCount++;
    f->flags.read = read;
    f->flags.compressed = z;
    sprintf(f->name, "0x%08X -> length: %u, read: %u compressed: %u", buf, size, read, z);
    return f;
}
