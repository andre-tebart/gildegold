/* 0x56d730, the run handler of the research event 0x96: like 0x56d3d0 but the record is 0x4eabe0 (the +0xb0 kind) and the clock is +0xc0; state 1 hands the result over (0x5a9bc0, 0x4ecc30 with +0xb4 + the record's +4), releases the building group and frees; state 0 takes the elapsed time off +0xb8 and retries after 5 until it is used up */

struct AHMSLOT * ahm_FindSlot();
int ai_Unk5a9bc0();
int ahm_SendSetPlaceValue(char *c, int id, float v);

void he_Unk56d730(int ev)
{
    char *c, *p;
    int d, st, h, v;

    d = gm_ClockSeason((void *)(unsigned)&ai_Time);
    p = 0;
    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0 || ahm_HasRecord(c) == 0 || (p = (char *)ahm_FindSlot(c, *(int *)(ev + 0xb0))) == 0)
        *(int *)(ev + 0x70) = -2;
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(float *)((unsigned)&ai_Unk617840 + d * 4) <= (float)*(unsigned short *)(unsigned)&ai_TimeHour ||
        (float)*(unsigned short *)(unsigned)&ai_TimeHour < *(float *)((unsigned)&ai_Unk617830 + d * 4)) {
        *(HETIME *)(ev + 0xc0) = ai_Time;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (he_Unk56ddc0(*(int *)(c + 0x16c), 0x24b) != 0) {
        *(HETIME *)(ev + 0xc0) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    }
    if (st == 1) {
        v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b);
        h = gm_FindChar(*(int *)(ev + 0xc));
        if (h != 0)
            ai_Unk5a9bc0(h, c, v, *(int *)(ev + 0xb0));
        ahm_SendSetPlaceValue(c, *(int *)(ev + 0xb0), *(float *)(ev + 0xb4) + *(float *)(p + 4));
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        v = gm_MinutesBetween((void *)(ev + 0xc0), (void *)(unsigned)&ai_Time);
        *(int *)(ev + 0xb8) -= v;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(HETIME *)(ev + 0xc0) = ai_Time;
        if (*(int *)(ev + 0xb8) < 1)
            *(int *)(ev + 0x70) = 1;
    }
}
