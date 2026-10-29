/* 0x42fa10, builds the path of entry e (or of directory d without the trailing slash if e is 0) relative to
   the directory stop.
   @deep behaviour: 56 code hunk lines are the ebp/ebx role swap of the out buffer and the loop directory pointer (every differing instruction is mov/cmp/lea/rep with the other register, e.g. [ebp+0x110] vs [ebx+0x110], mov eax,ebx vs mov eax,ebp, plus that mov sitting one slot later between pop edi and pop esi in the epilogue = order 18); no statement, constant, callee or operand differs (asmdiff, listing). Register/slot allocation - deep */

char *vfs_EntryPath(VFSENTRY *e, VFSDIR *d, VFSDIR *stop, char *out)
{
    char tmp[0x100];
    VFSDIR *p = d;
    char *r;

    if (!p)
        r = 0;
    else {
    r = out;
    if (p != stop) {
        strcpy(out, p->name);
        if (e) {
            strcat(out, e->name);
            while (p->parent && (p = p->parent) != stop) {
                strcpy(tmp, p->name);
                strcat(tmp, out);
                strcpy(out, tmp);
            }
        } else {
            out[strlen(out) - 1] = 0;
            while (p->parent && (p = p->parent) != stop) {
                strcpy(tmp, p->name);
                strcat(tmp, out);
                strcpy(out, tmp);
            }
        }
    } else
        strcpy(out, e->name);
    }
    return r;
}
