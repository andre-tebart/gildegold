/* 0x4ea7a0, reads the office table from the file f (amt_InitTable first); the saves up to the version 0x10050 hold 0x25
   records, later ones 0x2a ("amt_fio_LoadAemter(): count read is %i, expected %i" when the count does not fit); 1 when
   all was read */

int amt_LoadAemter(void *f, int version)
{
    unsigned int expect, i, count;
    char buf[256];

    expect = (((version < 0x10051) - 1) & 5) + 0x25;
    amt_InitTable();
    if (f == 0)
        return 0;
    if (ls_Read(&count, 4, 1, f) == 0)
        return 0;
    if (count != expect) {
        sprintf(buf, "amt_fio_LoadAemter(): count read is %i, expected %i", count, 0x2a);
        return 0;
    }
    for (i = 0; i < expect; i++) {
        if (ls_Read(&amt_Table[i].idx, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].owner, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].kind, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].cnt, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].state, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].x14, 4, 1, f) == 0)
            return 0;
    }
    return 1;
}
