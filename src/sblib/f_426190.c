/* 0x426190, allocates a sample slot for variation idx of the named type-2 entry (the bank entry is loaded
   on demand, the bank looked up when bank is NULL) and marks the slot as fixed-variation; returns the slot,
   NULL when the entry is no variation, idx is out of range or no bank owns it, &sb_Dummy when the driver is
   closed */

SBSAMPLE *sb_GetVariation(SBBANK *bank, char *name, int idx)
{
    SBSAMPLE *s;

    if (!sb_Driver)
        return &sb_Dummy;
    s = sb_AllocSample();
    if (s) {
        s->info = sb_FindEntry(bank, name);
        if (s->info) {
            if (!bank && !(bank = sb_GetBankOf(s->info)))
                return NULL;
            if (s->info->type == 2) {
                if (!s->info->data)
                    sb_LoadEntry(bank, s->info);
                if (s->info->data && idx < ((SBVAR *)s->info->data)->count) {
                    s->loops = 1;
                    s->pan = 0x3f;
                    s->flags |= 4;
                    s->volume = 0x7f;
                    s->var = idx;
                    return s;
                }
            }
        }
    }
    return NULL;
}
