/* 0x426db0, drops every loaded bank: the samples of every entry are stopped, the entry data and the bank
   blocks are freed and sb_Used is reduced by their sizes; sb_Banks ends up NULL. Does nothing when the
   driver is closed or no memory is used */

void sb_UnloadAll(void)
{
    SBBANK *bank, *next;
    SBSAMPLE *s;
    SBENTRY *e;
    int i, k;

    if (sb_Driver && sb_Used) {
        bank = sb_Banks;
        while (bank) {
            bank->refs = 0;
            for (i = 0; i < bank->count; i++) {
                e = &bank->entries[i];
                if (e->data) {
                    k = 0;
                    for (s = sb_FindSampleOf(e, &k); s; s = sb_FindSampleOf(&bank->entries[i], &k)) {
                        sb_Stop(s, 0);
                        s->info = NULL;
                        k++;
                    }
                    e = &bank->entries[i];
                    if (bank->entries[i].type == 1)
                        sb_Used -= ((SBDATA *)e->data)->len + 12;
                    else
                        sb_Used -= ((SBVAR *)e->data)->len + 12;
                    mem_Free(bank->entries[i].data);
                    bank->entries[i].data = NULL;
                    bank->entries[i].data = NULL;
                }
            }
            next = bank->next;
            sb_Used -= bank->count * sizeof(SBENTRY) + sizeof(SBBANK);
            mem_Free(bank);
            bank = next;
        }
        sb_Banks = NULL;
    }
}
