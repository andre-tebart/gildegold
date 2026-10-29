/* 0x5a86c0, the setting removed by the good (75 B): the list 0x6f26c0 is relinked and the record 0x422620
   freed; -1 when the good is not in the list */

int ai_Unk5a86c0(int param_1)
{
    int *prev;
    int *p;

    prev = 0;
    p = (int *)*(int *)((unsigned)&full_data_6f26c0);
    while (p != 0) {
        if (*p == param_1)
            break;
        prev = p;
        p = (int *)p[0x12];
    }
    if (p == 0)
        return -1;
    if (prev != 0)
        prev[0x12] = p[0x12];
    else
        *(int *)((unsigned)&full_data_6f26c0) = p[0x12];
    mem_Free((int)p);
    return 0;
}
