/* 0x5a8a20, the settings loaded (347 B): the list is freed (0x5a8920), the count read by 0x51c920, that many
   0x4c-byte records from 0x4223c0 ("hp_LoadProdSetting") linked at +0x48, then +0 / +4 / +0x44 and the seven
   slots read back; 1 done, 0 on a read error (the list is freed again) */

int ai_Unk5a8a20(void *param_1)
{
    char *rec;
    char *p;
    int n;
    int i;

    ai_FreeSettings();
    if (ls_Read(&n, 4, 1, param_1) == 0)
        return 0;
    for (i = 0; i < n; i++) {
        rec = (char *)mem_Alloc(0x4c, (char *)(unsigned)&full_data_62c298);
        if (rec == 0)
            goto fail;
        memset(rec, 0, 0x4c);
        *(int *)rec = -1;
        if (*(int *)((unsigned)&full_data_6f26c0) != 0)
            *(int *)(rec + 0x48) = *(int *)((unsigned)&full_data_6f26c0);
        *(int *)((unsigned)&full_data_6f26c0) = (int)rec;
    }
    p = (char *)*(int *)((unsigned)&full_data_6f26c0);
    if (p == 0)
        return 1;
    while (ls_Read(p, 4, 1, param_1) != 0 && ls_Read(p + 4, 4, 1, param_1) != 0 &&
           ls_Read(p + 0x44, 4, 1, param_1) != 0) {
        for (i = 0; i < 7; i++) {
            if (ls_Read(p + 0xc + i * 8, 4, 1, param_1) == 0)
                goto fail;
            if (ls_Read(p + 0x10 + i * 8, 1, 1, param_1) == 0)
                goto fail;
        }
        p = (char *)*(int *)(p + 0x48);
        if (p == 0)
            return 1;
    }
fail:
    ai_FreeSettings();
    return 0;
}
