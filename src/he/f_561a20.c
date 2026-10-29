/* 0x561a20, the run handler "Einbruch" (the burglary walk; 8 states by the jump table 0x5628bc; ROUGH): state 0 sends the burglars (the group 0x524260 (building, .., id, ev), 0x4c5230, 0x4c52e0 "Einbruch" 0x627a24) towards the building +0xac (none: state -1), state 1 waits for their arrival (0x524770, a figure count of 8) and goes on with the entry in 0x561c20; the clock is posted by 0x4fe140 */

void he_Unk561a20(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, i, p;

    if (st < 0 || st > 7)
        return;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    for (i = 0; i < 8; i++) {
        p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
        if (p == 0)
            continue;
        if (st == 0) {
            sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
            ai_Unk4c5230(*(int *)(p + 4));
            ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Einbruch");
        } else {
            sv_Unk524770(p, 0);
        }
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0x70) = st + 1;
}
