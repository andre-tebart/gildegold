/* 0x4ad010, the record with running number idx; *kind = 0x27 for a person, 0x26 for an event */

HMTREC *hmt_Find(int idx, int *kind)
{
    int i;

    for (i = 0; i < hmt_File.np; i++) {
        if (hmt_File.p[i].idx == idx) {
            if (kind != 0)
                *kind = 0x27;
            return &hmt_File.p[i];
        }
    }
    for (i = 0; i < hmt_File.nk; i++) {
        if (hmt_File.k[i].idx == idx) {
            if (kind != 0)
                *kind = 0x26;
            return &hmt_File.k[i];
        }
    }
    return 0;
}
