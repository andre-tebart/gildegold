/* 0x5a84b0, the record lookup in the list 0x6f26c0 by the id at +0 (the next is +0x48) (27 B) */

int *ai_Unk5a84b0(int param_1)
{
    int *p;

    p = (int *)*(int *)((unsigned)&full_data_6f26c0);
    while (p != 0) {
        if (*p == param_1)
            return p;
        p = (int *)p[0x12];
    }
    return 0;
}
