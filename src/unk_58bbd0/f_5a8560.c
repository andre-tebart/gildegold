/* 0x5a8560, the setting of a good, created on demand (338 B): the first record is created without a link, a
   later one by 0x5a8540; the 0x4c bytes from 0x4223c0 ("hp_AddProdSetting") are zeroed, the good +4 of
   param_1 at +0, 0x5de634 at +4, 1.0f at +0x44 and the weights by 0x5a83b0 */

void *ai_Unk5a8560(int param_1)
{
    char *rec;
    void *found;

    if (*(int *)((unsigned)&full_data_6f26c0) == 0) {
        if (ai_Unk5a84d0((char *)param_1) == 0)
            return 0;
        rec = (char *)mem_Alloc(0x4c, (char *)(unsigned)&full_data_62c284);
        if (rec == 0)
            return 0;
        memset(rec, 0, 0x4c);
        *(int *)rec = *(int *)(param_1 + 1);
        *(int *)(rec + 4) = rand();
        *(float *)(rec + 0x44) = 1.0f;
        ai_Unk5a83b0(param_1, rec);
        *(int *)((unsigned)&full_data_6f26c0) = (int)rec;
        return rec + 0xc;
    }
    found = ai_Unk5a8540(*(int *)(param_1 + 1));
    if (found != 0)
        return found;
    if (ai_Unk5a84d0((char *)param_1) == 0)
        return 0;
    rec = (char *)mem_Alloc(0x4c, (char *)(unsigned)&full_data_62c284);
    if (rec == 0)
        return 0;
    memset(rec, 0, 0x4c);
    *(int *)rec = *(int *)(param_1 + 1);
    *(int *)(rec + 4) = rand();
    *(float *)(rec + 0x44) = 1.0f;
    ai_Unk5a83b0(param_1, rec);
    *(int *)(rec + 0x48) = *(int *)((unsigned)&full_data_6f26c0);
    *(int *)((unsigned)&full_data_6f26c0) = (int)rec;
    return rec + 0xc;
}
