/* 0x426350, the loaded bank entry used longest ago (lowest SB_TIME, data present) whose samples have all
   finished, so its data can be freed; returns &sb_DummyEntry when the driver is closed and NULL when
   nothing is loaded */

SBENTRY *sb_FindOldestEntry(void)
{
    SBENTRY *e = NULL;
    SBBANK *bank = sb_Banks;
    SBSAMPLE *s;
    int i, k;

    if (!sb_Driver)
        return &sb_DummyEntry;
    if (bank) {
        do {
            i = 0;
            if (!e) {
                for (; i < bank->count; i++) {
                    if (bank->entries[i].data) {
                        e = &bank->entries[i];
                        break;
                    }
                }
            }
            for (; i < bank->count; i++)
                if (e->used > bank->entries[i].used && bank->entries[i].data)
                    e = &bank->entries[i];
            bank = bank->next;
        } while (bank);
        if (e && (k = 0, s = sb_FindSampleOf(e, &k)) != NULL) {
            while (1) {
                if (sfx_SampleStatus(s->handle) == 4 || (s->flags | 0x10) != 0) {
                    e = NULL;
                    break;
                }
                s->info = NULL;
                k++;
                if (!(s = sb_FindSampleOf(e, &k)))
                    break;
            }
        }
    }
    return e;
}
