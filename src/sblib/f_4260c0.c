/* 0x4260c0, finds a bank entry by name (case-insensitive, at most 50 chars) in the given bank, or in every
   loaded bank when bank is NULL; returns the entry, NULL when nothing matches, &sb_DummyEntry when the
   driver is closed */

SBENTRY *sb_FindEntry(SBBANK *bank, char *name)
{
    int i = 0;
    SBENTRY *e = NULL;
    SBBANK *b;
    int n;

    if (!sb_Driver)
        return &sb_DummyEntry;
    if (bank) {
        n = bank->count;
        for (; i < n; i++)
            if (!_strnicmp(bank->entries[i].name, name, 50)) {
                e = &bank->entries[i];
                break;
            }
    } else {
        for (b = sb_Banks; b && !(e = sb_FindEntry(b, name)); b = b->next)
            ;
    }
    return e;
}
