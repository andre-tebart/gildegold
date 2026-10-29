/* 0x4ea690, saves the office table to the file f: the count 0x2a, then per record idx (1), owner (4), kind (1), cnt (4),
   state (1), x14 (4); 1 when all was written (the original's "amt_fio_SaveAemter")
   @deep B3: 31 code + 3 fill hunks, 47/272 bytes differ, our size = the original's 0x110 (142 code hunks at session start). The original 0x4ea690..0x4ea792 was read (disasm) and the statement sequence now matches: count 0x2a, the f == 0 guard, the count write with the 'amt_fio_SaveAemter(): count is %i' sprintf, and the 6 field writes of all 42 records in the original order (idx 1, owner 4, kind 1, cnt 4, state 1, x14 4) with return 0 on the first failed write and return 1 after the walk. Meaning/shape fixes this session: the index loop (i / i > 41) became the pointer walk ('for (p = amt_Table; (int)p < (int)&amt_Table[42]; p++)'; the original's signed jl against &amt_Table[42], esi walking 0x18 at a time), and the early-return shape ('if (vfs_fwrite(&count, 4, 1, f) == 0) return 0;' with the sprintf and the loop at the function's top level (the original's inline pop/xor eax/ret epilogues: its own copy for f == 0 at 0x4ea6ab, the count write and the six loop returns sharing 0x4ea6cc), which took 64 -> 31 hunks and 183 -> 47 bytes. Left: allocation only - the pointer base the loop register holds: the original keeps esi = &record->owner (idx at [esi-4], x14 at [esi+0x10], the loop test compares esi with 0x925514 = &amt_Table[42].owner), ours keeps esi = &record->kind (idx at [esi-8], the test lea edx,[esi-8] then cmp the bound). Same source try: index walk, do-while, for, and the p = amt_Table / p++ spellings give the same base. Q1-Q7 pass. */

int amt_SaveAemter(void *f)
{
    AMTREC *p;
    int count;
    char buf[256];

    count = 0x2a;
    if (f == 0)
        return 0;
    if (vfs_fwrite(&count, 4, 1, f) == 0)
        return 0;
    sprintf(buf, "amt_fio_SaveAemter(): count is %i", count);
    for (p = amt_Table; (int)p < (int)&amt_Table[42]; p++) {
        if (vfs_fwrite(&p->idx, 1, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->owner, 4, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->kind, 1, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->cnt, 4, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->state, 1, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->x14, 4, 1, f) == 0)
            return 0;
    }
    return 1;
}
