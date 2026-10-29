/* 0x4b4040, the dialog "Menu\CHOOSEHISTORY": picks the mission kind ai_MissionMode (0..5, the first button:
   none) and then a mission (0x4b3e70); returns its type (0xff: cancelled)
   @deep behaviour: instructions identical under the esi<->edi renaming (tools/asmdiff.py --regs esi=edi,edi=esi leaves only the prologue/epilogue push/pop order), the 82-instruction code hunk at 0x4b4094 is not classified regs only because the joined hunk holds 7 registers (diffclass find_map limit 6). The swapped candidates are view (ours edi, original esi) and the first/o5/g colour (ours esi, original edi); c2prio: _first prio 182 -> esi, _view prio 181 -> edi, a 1-point allocation difference (the original ranks view above first). Last-bytes pass: spell.py dup-tail 0x4b4094 (no if/else arm at that block - difflib noise from the swap), push-pop 0x4b421f (one final return already; the original's je to the loop test is matched), order 0x4b4265 (same instruction set after the renaming); no meaning-class lead, no source form left. Tried: declaration order 'first, view' (no effect on C2 priorities). Readability done (Q1/Q2 cleared, Q6 present, Q7 clean). Left: C2 register-priority / colouring work (c2prio / vargen ren). */

unsigned char ai_NumPlayers = 1;  /* 0x62489c */
void d2_RadioSelect(int g, int sel);                                             /* 0x46c630 */

int ai_ChooseMission(void)
{
    int o0, o1, o2, o3, o4, o5, o6;
    int view, first, g, r;

    r = 0;
    o6 = -1;
    view = d2_OpenForm(0, 0, "Menu\\CHOOSEHISTORY");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    first = io_PrintText((char *)0x1ac5, 0x1ac7, 0x1ac8, 0x1ac9, 0x1aca, 0x1acb, 0x1acc);
    o0 = d2_GetWinObj(view, 0, first);
    o1 = d2_GetWinObj(view, 0, first + 1);
    o2 = d2_GetWinObj(view, 0, first + 2);
    o3 = d2_GetWinObj(view, 0, first + 3);
    o4 = d2_GetWinObj(view, 0, first + 4);
    o5 = d2_GetWinObj(view, 0, first + 5);
    if (ai_NumPlayers > 1)
        o6 = d2_GetWinObj(view, 0, io_PrintText("%it[%s]", 0, 0x1acd));
    g = d2_CreateRadioGroup(6, o0, o1, o2, o3, o4, o5);
    d2_RadioSelect(g, 0);
    d2_RadioAdd(g, d2_GetWinObj(view, 0, io_PrintText((char *)0x7e)));
    while (sv_RunFrame((void (*)(void))ai_ChooseMission, sv_FrameMask)) {
        d2_MenuNavigate(g);
        if (dm_L.rup != 0 || kb_Last == 1 || d2_ClickedId == 1) {
            r = 0xff;
            sv_PopRequest = 1;
        }
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == o0) {
                ai_MissionMode = -1;
                r = 0;
            } else {
                if (d2_PointerObj == o1)
                    ai_MissionMode = 0;
                else if (d2_PointerObj == o2)
                    ai_MissionMode = 1;
                else if (d2_PointerObj == o3)
                    ai_MissionMode = 2;
                else if (d2_PointerObj == o4)
                    ai_MissionMode = 3;
                else if (d2_PointerObj == o5)
                    ai_MissionMode = 4;
                else if (d2_PointerObj == o6)
                    ai_MissionMode = 5;
                d2_RadioSelect(g, 0);
                r = ai_ChooseMissionType(ai_MissionMode);
                if (r == 0xff)
                    continue;
            }
            sv_PopRequest = 1;
        }
    }
    d2_RadioClear(g);
    d2_CloseView(view);
    return r;
}
