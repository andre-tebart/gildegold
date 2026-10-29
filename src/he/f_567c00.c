/* 0x567c00, the run handler of the tips (ROUGH): state 0 shows the next of the 27 tip texts (0x1442430 [0x6fa1e0 [+0xac]]) while the tips 0x6f366b are on, the next run in random(4) + 8 units, else in 1 unit */

void he_Unk567c00(int ev)
{
    int st, i;
    unsigned r;

    st = *(int *)(ev + 0x70);
    if (-3 < st) {
        if (st < 0) {
            he_Unk53e1f0((int *)ev);
        } else if (st == 0) {
            if (*(char *)((unsigned)&ai_GamePanel + 0x3) != 0) {
                he_ShowNews(*(int *)((unsigned)&ai_CharInts + U_I((unsigned)&ai_PlayerId) * 0x21c), -1, *(int *)((unsigned)&full_data_1442430 + *(int *)((unsigned)&full_data_6fa1e0 + *(int *)(ev + 0xac) * 4) * 4), 0, 0x5f9, 0);
                i = *(int *)(ev + 0xac) + 1;
                *(int *)(ev + 0xac) = i;
                if (0x1a < i)
                    *(int *)(ev + 0x70) = 1;
                *(HETIME *)(ev + 0x52) = ai_Time;
                r = ((int (*)(int, int, int))util_Random)(4, 0, 0);
                gm_AddTime(ev + 0x52, (r & 0xffff) + 8);
                return;
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
        }
    }
}
