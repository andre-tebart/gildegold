/* 0x4ef6b0, the index of the entry e (0x14 bytes, e[0] the list number 0 .. 3) within its list of the record r; -1 for another list */

int gv_EntryIndex(char *r, int *e)
{
    int base;

    base = e[0];
    if (base == 0)
        base = *(int *)(r + 8);
    else if (base == 1)
        base = *(int *)(r + 0xc);
    else if (base == 2)
        base = *(int *)(r + 0x10);
    else {
        if (base != 3)
            return -1;
        base = *(int *)(r + 0x14);
    }
    return ((int)e - base) / 0x14;
}
