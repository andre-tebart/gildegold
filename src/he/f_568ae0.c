/* 0x568ae0, the allocation handler of a building event (a Ghidra miss; ROUGH): +0xb0 = the value at 0x91aaf0 [+0xac * 0x41 + 0x22] (+0xb4 as a float), the clocks +0x52 / +0x60 are the start clock (+0x44), 0x50c980 (player id, 8, +0x8c) - 0.5 times 0.25 is the float +0xc4,
   +0xc8 = 0; every player record whose +0x17c is this event is locked (0x524260 (object of +0xb8, id, 0, ev)); the first run is one unit ahead */

double gm_Unk50c980();

void he_Unk568ae0(int ev)
{
    int v, o, rec;
    double d;

    v = *(int *)(U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41 + 0x22);
    *(int *)(ev + 0xb0) = v;
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(float *)(ev + 0xb4) = (float)v;
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x44);
    d = gm_Unk50c980(*(unsigned short *)(ev + 8), 8, ev + 0x8c) - 0.5;
    *(char *)(ev + 0xc8) = 0;
    *(float *)(ev + 0xc4) = (float)(d * 0.25);
    o = gm_FindBuildingById(*(int *)(ev + 0xb8));
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            sv_MoveToGroup(o, *(unsigned short *)rec, 0, ev);
    }
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
