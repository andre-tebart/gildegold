/* 0x426660, loads the data of a bank entry from the bank file (type 1: one sample, type 2: a variation set
   with its items), frees older data first when sb_Mem would be exceeded; returns 0, or -1 on failure */

int sb_LoadEntry(SBBANK *bank, SBENTRY *e)
{
    void *p = NULL;
    int off = 0;
    int ret = 0;
    FILE *f;
    SBDATA hdr, *d;
    SBVAR vhdr, *v;
    int i;

    if (!sb_Driver)
        return 0;
    if (!e->data && bank) {
        f = fopen(bank->path, "rb");
        if (f) {
                if (fseek(f, e->offset, SEEK_SET) == 0) {
                    if (e->type == 1) {
                        if (fread(&hdr, 12, 1, f) == 1) {
                            if (sb_Mem - sb_Used < hdr.len + 12)
                                sb_FreeMemory(hdr.len + 12);
                            p = d = mem_Alloc(hdr.len + 12, "sb_LoadEntry");
                            if (d) {
                                d->fmt = hdr.fmt;
                                d->len = hdr.len;
                                d->data = (char *)(d + 1);
                                /* reading into (d + 1) / d->len, not d->data / hdr.len, is what gives the original's store order */
                                if (fread((char *)(d + 1), d->len, 1, f) == 1) {
                                    e->used = SB_TIME;
                                    sb_Used += hdr.len + 12;
                                } else {
                                    mem_Free(p);
                                    p = NULL;
                                    ret = -1;
                                }
                            } else
                                sprintf(sb_Msg, "sb_LoadEntry: m_alloc schlug fehl f\xfcr %s", e->name);
                        } else
                            ret = -1;
                        e->data = p;
                    } else if (e->type == 2 && fread(&vhdr, 12, 1, f) == 1) {
                        if (sb_Mem - sb_Used < vhdr.len + 12)
                            ret = sb_FreeMemory(vhdr.len + 12);
                        p = v = mem_Alloc(vhdr.len + 12, "sb_LoadEntry");
                        if (v) {
                            v->count = vhdr.count;
                            v->len = vhdr.len;
                            v->items = (SBDATA *)(v + 1);
                            if (fread((SBDATA *)(v + 1), v->len, 1, f) == 1) {
                                for (i = 0; i < v->count; i++) {
                                    v->items[i].data = (char *)v->items + v->count * 12 + off;
                                    off += v->items[i].len;
                                }
                                sb_Used += vhdr.len + 12;
                                e->used = SB_TIME;
                                sprintf(sb_Msg, "Variation %s loaded ! MemUsed is now %d bytes.", e->name, sb_Used);
                            } else {
                                mem_Free(p);
                                p = NULL;
                                ret = -1;
                            }
                        } else
                            ret = -1;
                        e->data = p;
                    } else
                        ret = -1;
                } else
                    ret = -1;
            fclose(f);
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
