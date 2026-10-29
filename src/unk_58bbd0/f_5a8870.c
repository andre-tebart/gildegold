/* 0x5a8870, a good drawn by the weights of a setting (111 B): the record of 0x5a84b0 (+4 of param_1), the
   random of 0x5a8340 * 100.0f (0x5ebb40) against the running sum of the weights +0x10, the good of the first
   slot over it */

int ai_DrawSettingGood(int param_1)
{
    int rec;
    int *p;
    int i;
    int draw;
    int sum;
    int pick;

    rec = ai_Unk5a84b0(*(int *)(param_1 + 1));
    if (rec == 0)
        return 0;
    draw = (int)((float)ai_Unk5a8340(rec) * *(float *)((unsigned)&full_rdata_5ebb40));
    sum = 0;
    pick = 0;
    i = 0;
    p = (int *)(rec + 0xc);
    do {
        if (pick != 0)
            return pick;
        if (*p != 0) {
            sum += *(unsigned char *)(p + 1);
            if (*(unsigned char *)(p + 1) != 0 && draw < sum)
                pick = *p;
        }
        i++;
        p += 2;
    } while (i < 7);
    return pick;
}
