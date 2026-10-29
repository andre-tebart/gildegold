/* 0x426ba0, drops one reference of bank and, when the count reaches 0, stops the samples of its entries,
   frees their data and the bank itself (sb_Used is reduced by the data sizes and the bank block); returns 0,
   1 when the driver is closed and -1 when nothing is used or bank is NULL */

int sb_UnloadSampleBank(SBBANK *bank)
{
    int ret = 0;
    SBBANK *b, *prev = NULL;
    SBSAMPLE *s;
    SBENTRY *e;
    int i, k;
    char *path;

    if (!sb_Driver)
        return 1;
    if (sb_Used && bank) {
        for (b = sb_Banks; b; b = b->next) {
            if (b == bank) {
                if (--b->refs <= 0) {
                    if (prev)
                        prev->next = b->next;
                    else
                        sb_Banks = b->next;
                    path = bank->path;
                    sprintf(sb_Msg, "Unloading Samplebank %s ... MemUsed before unloading %d bytes.", path, sb_Used);
                    for (i = 0; i < b->count; i++) {
                        e = &b->entries[i];
                        if (b->entries[i].data) {
                            k = 0;
                            for (s = sb_FindSampleOf(e, &k); s; s = sb_FindSampleOf(&b->entries[i], &k)) {
                                sb_Stop(s, 0);
                                s->info = NULL;
                                k++;
                            }
                            e = &b->entries[i];
                            if (b->entries[i].type == 1)
                                sb_Used -= ((SBDATA *)e->data)->len + 12;
                            else
                                sb_Used -= ((SBVAR *)e->data)->len + 12;
                            mem_Free(b->entries[i].data);
                            b->entries[i].data = NULL;
                            b->entries[i].data = NULL;
                        }
                    }
                    sb_Used -= b->count * sizeof(SBENTRY) + sizeof(SBBANK);
                    sprintf(sb_Msg, "Finished unloading Samplebank %s ... MemUsed after unloading %d bytes.", path, sb_Used);
                    mem_Free(b);
                }
                break;
            }
            prev = b;
        }

    } else
        ret = -1;
    return ret;
}
