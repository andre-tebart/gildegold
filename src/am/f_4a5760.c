/* 0x4a5760: for n wanted items rec[i].item, finds them in the character's inventory list (0x178, next at 0x3f;
   goods of kind 9 skipped): rec[i].unk08 the entry's id, rec[i].unk0c the entry, rec[i].unk04 its 0x0e value when
   usable (0x5ba620 class matches 0x1ec or 0x5ba6b0 refuses), else -1. st[0] counts the entries, st[1] the
   unusable, st[2] the usable ones */

void ai_Unk4a5760(G5CH *p, int n, G9REC *rec, int *st)
{
    unsigned short *e;
    G9REC *r;
    unsigned char *c;
    int i, v;

    if (n > 0) {
        for (r = rec, i = n; i != 0; i--, r++) {
            r->unk08 = -1;
            r->unk04 = 0;
        }
    }
    st[1] = 0;
    st[2] = 0;
    st[0] = 0;
    for (e = *(unsigned short **)((unsigned char *)p + 0x178); e != 0; e = *(unsigned short **)((unsigned char *)e + 0x3f)) {
        if (ai_Goods[*e].type == 9)
            continue;
        st[0]++;
        for (i = 0, r = rec; i < n; i++, r++) {
            if (*e != (unsigned int)r->item)
                continue;
            c = (unsigned char *)G8_5BA620(*e);
            if (c == 0) {
                v = -1;
                r = &rec[i];
                r->unk04 = -1;
                st[1]++;
                r->unk0c = 0;
            } else {
                if (!(*(unsigned int *)(c + 0x10) & *(unsigned int *)((unsigned char *)p + 0x1ec))
                    && ai_InventoryHas(p, *e) == 0) {
                    st[2]++;
                    rec[i].unk04 = *(int *)(e + 7);
                    rec[i].unk08 = *(int *)(e + 1);
                    rec[i].unk0c = (int)e;
                    break;
                }
                r = &rec[i];
                r->unk04 = -1;
                st[1]++;
                v = *(int *)(e + 1);
                r->unk0c = (int)e;
            }
            r->unk08 = v;
            break;
        }
    }
    for (; n > 0; n--, rec++) {
        if (rec->unk08 == -1 && (c = (unsigned char *)G8_5BA620(rec->item)) != 0
            && (*(unsigned int *)(c + 0x10) & *(unsigned int *)((unsigned char *)p + 0x1ec)))
            st[1]++;
    }
}
