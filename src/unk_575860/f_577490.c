/* 0x577490, releases the pending id S+0x50: with the kind S+0x40 2 through 0x52f9f0, with the kind 3 through
   0x52fa00; returns 0 */

int ai_Unk577490(void)
{
    char *st;

    st = S;
    if (*(int *)(st + 0x40) == 2 && *(int *)(st + 0x50))
        sv_Unk52f9f0(*(int *)(st + 0x50));
    else if (*(int *)(st + 0x40) == 3 && *(int *)(st + 0x50))
        sv_Unk52fa00(*(int *)(st + 0x50));
    return 0;
}
