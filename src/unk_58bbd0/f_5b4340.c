/* 0x5b4340, a list window of records (580 B, ROUGH): param_1 (its word -1 = empty), param_2 the query record;
   0x4f3fe0 (the +4 of param_2, the id array, the word of param_1) fills the ids, then each id 0x91f700 + id *
   0x2d (a 0x2d-byte record) is copied into the local array and 0x4f4390 (count, the array) builds the list;
   the window "0x62cd7c" (0x4741f0 / 0x475600 / 0x475f70) with its control and pump loop follows */

int ai_Unk5b4340(short *param_1, int param_2)
{
    int ids[0x10];
    char list[0x2d * 0x10];
    int n;
    int i;
    int win;

    if (*param_1 == -1)
        return 0;
    n = unk_Unk4f3fe0(*(int *)(param_2 + 4), ids, *param_1);
    if (n < 1)
        return 0;
    for (i = 0; i < n; i++)
        memcpy(list + i * 0x2d, (void *)((unsigned)&ai_Unk91f700 + U_I((int)ids + i * 4) * 0x2d), 0x2d);
    unk_Unk4f4390(n, list);
    win = d2_OpenForm((char *)(unsigned)&full_data_62cd7c, 0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    /* ROUGH: the list control, the texts and the pump loop 0x5309a0 */
    return 1;
}
