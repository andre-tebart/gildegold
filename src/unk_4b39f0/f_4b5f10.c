/* 0x4b5f10, the mission result dialog of mission 0 (0x4b5460, text 0x1b95); then the mission again (0x4b5360),
   the mission end dialog (0x4b58b0, game mode not 5) or quitting (0x4b5950, ai_GameOver)
   @deep behaviour draft (readable): a pure callee-saved colour swap. asmdiff --regs ebp=ebx,ebx=ebp leaves only the push order of the ebp/ebx pair, the [ebp+0] vs [ebx] addressing byte, our 32-bit or ebp,0x80 against the original's or bl,0x80 (consequence: no bpl byte form in 32-bit) and the 4-byte tail fill. Read: c (&ai_Chars[ai_PlayerId]) and t (ai_GetMissionType) are the two swapped candidates; ours c=ebp/t=ebx, the original c=ebx/t=ebp; flags and the -1 of d2_ClickedId inherit c's register. spell.py menu has no regs entry; leads tried, one edit + one build each: t declared before c (allocation unchanged, reverted), t assigned before c (c -> ebx but the ai_GetMissionType call moves ahead of the lea and the prologue reorders: 20 -> 32 differing instructions, reverted). c2prio needs frida (not installed). The swap needs the priority/slot work. */

int ai_MissionPartner = -1;  /* 0x6248a0, the mission partner (-1 none) */

void ai_MissionResultDialog(int id)
{
    AICHAR *c;
    G13TYPE *t;
    int r, view, flags;

    r = 0;
    c = &ai_CharsT[ai_PlayerId];
    t = ai_GetMissionType(ai_Missions[0].type);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    id = (id & ~9) | 0x100;
    ai_MissionSuccessDialog(t, id);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b95, c->id);
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (ai_MissionPartner == -1) {
            if (ai_GameOver)
                r = 3;
            else
                r = (ai_MissionMode != 5) + 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionResultDialog, flags));
    d2_CloseView(view);
    if (r == 1)
        ai_MissionInfoDialog(ai_Missions[0].type, id);
    else if (r == 2)
        ai_MissionEndDialog(id);
    else if (r == 3)
        ai_QuitGame(id);
}
