/* 0x427050, allocates a sample slot for variation idx of the named type-2 entry (the entry is loaded on
   demand, the bank looked up when bank is NULL), sets loops / pan / volume / the fixed-variation flag and
   starts it with the fade; returns the slot, NULL when the entry is no loaded variation or idx is out of
   range, &sb_Dummy when the driver is closed */

SBSAMPLE *sb_PlayVariation(SBBANK *bank, char *name, int idx, int loops, int pan, int volume, int fade)
{
    SBSAMPLE *s;
    SBENTRY *e;

    if (!sb_Driver)
        return &sb_Dummy;
    s = sb_AllocSample();
    if (s) {
        e = s->info = sb_FindEntry(bank, name);
        if (e && (bank || (bank = sb_GetBankOf(e))) && e->type == 2) {
            if (!e->data)
                sb_LoadEntry(bank, e);
            if (s->info->data && idx < ((SBVAR *)s->info->data)->count) {
                s->loops = loops;
                s->pan = pan;
                s->volume = volume;
                s->flags |= 4;
                s->var = idx;
                sb_Play(s, fade, 0);
            } else
                s = NULL;
        } else
            s = NULL;
    }
    return s;
}
