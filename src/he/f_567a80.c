/* 0x567a80, the run handler of the tutorial hints (ROUGH): state 0 shows the hint of the entry +0xac of the table 0x6fa260 (the text 0x143a1c0 [id] through 0x53dda0 to the local player, when hints 0x6f3669 are on), steps
   to the next entry and sets the next run to its due clock; an empty entry ends (state 1); -1 / -2 free */

void he_Unk567a80(int ev)
{
    int st, v, i;

    st = *(int *)(ev + 0x70);
    if (-3 < st) {
        if (st < 0) {
            he_Unk53e1f0((int *)ev);
        } else if (st == 0) {
            v = *(int *)((unsigned)&full_data_6fa26c + *(int *)(ev + 0xac) * 16);
            if (v != 0 && v != -1 && *(char *)((unsigned)&ai_GamePanel + 0x1) != 0)
                he_ShowNews(*(int *)((unsigned)&ai_CharInts + U_I((unsigned)&ai_PlayerId) * 0x21c), -1, *(int *)((unsigned)&d2_Strings + v * 4), 0x40, 0x5f9, 0);
            i = *(int *)(ev + 0xac) + 1;
            *(int *)(ev + 0xac) = i;
            if (*(int *)((unsigned)&full_data_6fa26c + i * 16) == 0) {
                *(int *)(ev + 0x70) = 1;
                return;
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x52) = *(int *)((unsigned)&full_data_6fa260 + i * 16);
            *(short *)(ev + 0x56) = *(short *)((unsigned)&full_data_6fa264 + i * 16);
            *(int *)(ev + 0x58) = *(int *)((unsigned)&full_data_6fa268 + i * 16);
        }
    }
}
