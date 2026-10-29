/* 0x4ecf10, writes the record of the object o (+0x65; a blank one with id -1 when it has none) to the file f: id (4), float (4), 4 shares (1 each), 6 places (2 x 4); 1 when all was written @deep B3 draft, 133 differing instructions: the record layout (id, float, 4 shares, 6 place pairs) is proven and the body is complete; the hunks are the prologue (the original pushes ebp and keeps the record pointer in ebx, ours in esi without ebp), the 0x3c local's frame slot, and the position of the shared return-0 tail (the original inlines pop edi/pop esi/pop ebp/xor eax/pop ebx/ret after every failed fwrite, ours jumps to one shared tail with jbe against the original's ja over it). Meaning lead applied this session: the four flat fwrite checks are the unsigned <= 0 form (the unit's vfs_fwrite is declared unsigned; 135 -> 133 differing instructions), the nested +0x11 / loop checks are > 0. Reverted: the same change on the nested checks and the while (191 instructions - the inverted nested block regroups the whole body). spell.py: no menu rule for the remaining hunks (prologue / frame slot / tail placement). */


int ahm_WriteRecord(void *f, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    int *r;
    int rec[15];
    int i;

    r = (int *)obj->rec;
    memset(rec, 0, 0x3c);
    rec[0] = -1;
    if (r == 0)
        r = rec;
    if (vfs_fwrite(r, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(r + 1, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(r + 2, 1, 1, f) != 0) {
        if (vfs_fwrite((char *)r + 9, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite((char *)r + 10, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite((char *)r + 11, 1, 1, f) != 0) {
            i = 0;
            r += 4;
            while (vfs_fwrite(r - 1, 4, 1, f) != 0) {
                if (vfs_fwrite(r, 4, 1, f) <= 0)
                    return 0;
                i++;
                r += 2;
                if (i > 5)
                    return 1;
            }
        }
        return 0;
    }
    return 0;
}
