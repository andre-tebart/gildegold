/* 0x56d3d0, the run handler of the research event 0x95: states -2 / -1 free; in the night of the day (the float ranges 0x617830 / 0x617840) the clock +0xbc is reset and the event retried after 1; state 1 hands the result over (0x5a9c30, 0x4ecaa0), releases the building group and frees; state 0 waits for the research (0x56ddc0), takes the elapsed time from +0xbc (0x4fe280) off the amount +0xb4 and retries after 5 until the amount is used up (state 1) */

int gm_ClockSeason();
int ahm_HasRecord();
int ai_Unk5a9c30();
int ahm_SendAddPlace();
int gm_MinutesBetween();
int he_Unk56ddc0();

void he_Unk56d3d0(int ev)
{
    char *c;
    int d, st, h, v;

    d = gm_ClockSeason((void *)(unsigned)&ai_Time);
    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0 || ahm_HasRecord(c) == 0)
        *(int *)(ev + 0x70) = -2;
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(float *)((unsigned)&ai_Unk617840 + d * 4) <= (float)*(unsigned short *)(unsigned)&ai_TimeHour ||
        (float)*(unsigned short *)(unsigned)&ai_TimeHour < *(float *)((unsigned)&ai_Unk617830 + d * 4)) {
        *(HETIME *)(ev + 0xbc) = ai_Time;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (st == 1) {
        v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0);
        h = gm_FindChar(*(int *)(ev + 0xc));
        if (h != 0)
            ai_Unk5a9c30(h, c, v, *(int *)(ev + 0xb0));
        ahm_SendAddPlace(c, *(int *)(ev + 0xb0));
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (he_Unk56ddc0(*(int *)(c + 0x16c), 0x24b) != 0) {
        *(HETIME *)(ev + 0xbc) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    }
    if (st == 0) {
        v = gm_MinutesBetween((void *)(ev + 0xbc), (void *)(unsigned)&ai_Time);
        *(int *)(ev + 0xb4) -= v;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(HETIME *)(ev + 0xbc) = ai_Time;
        if (*(int *)(ev + 0xb4) < 1)
            *(int *)(ev + 0x70) = 1;
    }
}
