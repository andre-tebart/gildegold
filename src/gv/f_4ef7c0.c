/* 0x4ef7c0, the first entry (0x14 bytes) of the list l (0 .. 3, 0xff = the list at +0x1c) of the record of the object o, after the entry `after` (0: from the start), for which cb (a, entry) is true (no cb: any);
   the list ends at the table limit unk_Tab5ee084[l][kind] (0xff: 0x4ef470 entries); 0 when none or the list is missing (0x4ef4f0) */

unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l)
{
    char *rec;
    char *b;
    unsigned end, cur;
    int n;

    b = ai_BTypes + o[0] * 0x24d;
    rec = *(char **)(o + 0x65);
    if (l != 0xff) {
        if (unk_Unk4ef4f0(rec, l) == 0)
            return 0;
    }
    if (l == 0)
        end = *(int *)(rec + 8) + unk_Tab5ee084[0][(unsigned char)b[0x247]] * 0x14;
    else if (l == 1)
        end = *(int *)(rec + 0xc) + unk_Tab5ee084[1][(unsigned char)b[0x247]] * 0x14;
    else if (l == 2)
        end = *(int *)(rec + 0x10) + unk_Tab5ee084[2][(unsigned char)b[0x247]] * 0x14;
    else if (l == 3)
        end = *(int *)(rec + 0x14) + unk_Tab5ee084[3][(unsigned char)b[0x247]] * 0x14;
    else {
        if (l != 0xff)
            return 0;
        n = unk_Unk4ef470(o);
        end = *(int *)(rec + 0x1c) + n * 0x14;
    }
    if (after == 0) {
        if (l == 0)
            cur = *(unsigned *)(rec + 8);
        else if (l == 1)
            cur = *(unsigned *)(rec + 0xc);
        else if (l == 2)
            cur = *(unsigned *)(rec + 0x10);
        else if (l == 3)
            cur = *(unsigned *)(rec + 0x14);
        else
            cur = *(unsigned *)(rec + 0x1c);
    } else
        cur = after + 0x14;
    while (1) {
        if (end <= cur)
            return 0;
        if (cb == 0 || cb(a, cur) != 0)
            break;
        cur += 0x14;
    }
    return cur;
}
