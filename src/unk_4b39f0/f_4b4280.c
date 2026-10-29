/* 0x4b4280, 0x4b4040 with the first n kinds disabled (0x475e40) and a cancel button (returns 0xff); passes
   0x4b4040 to 0x5309a0 (sic) */

void d2_SetObjEnabled(int obj, int on);                                              /* 0x475e40 */

int ai_ChooseMissionFrom(int n)
{
    int o0, o1, o2, o3, o4, o5, o6;
    int view, first, g, r, sel;

    r = 0;
    view = d2_OpenForm(0, 0, "Menu\\CHOOSEHISTORY");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    first = io_PrintText((char *)0x1b9f);
    o0 = d2_GetWinObj(view, 0, first);
    o1 = d2_GetWinObj(view, 0, first + 1);
    o2 = d2_GetWinObj(view, 0, first + 2);
    o3 = d2_GetWinObj(view, 0, first + 3);
    o4 = d2_GetWinObj(view, 0, first + 4);
    o5 = d2_GetWinObj(view, 0, first + 5);
    o6 = d2_GetWinObj(view, 0, first + 6);
    g = d2_CreateRadioGroup(7, o0, o1, o2, o3, o4, o5, o6);
    sel = 1;
    if (n > 0) {
        d2_SetObjEnabled(o1, 0);
        sel = 2;
    }
    if (n > 1) {
        d2_SetObjEnabled(o2, 0);
        sel++;
    }
    if (n > 2) {
        d2_SetObjEnabled(o3, 0);
        sel++;
    }
    if (n > 3) {
        d2_SetObjEnabled(o4, 0);
        sel++;
    }
    if (n > 4) {
        d2_SetObjEnabled(o5, 0);
        sel = 0;
    }
    d2_RadioSelect(g, sel);
    while (sv_RunFrame((void (*)(void))ai_ChooseMission, sv_FrameMask)) {
        d2_MenuNavigate(g);
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == o6) {
                ai_MissionMode = -1;
                r = 0xff;
            } else if (d2_PointerObj == o0) {
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
                d2_RadioSelect(g, 0);
                r = ai_ChooseMissionType(ai_MissionMode);
                if (r == 0 || r == 0xff)
                    continue;
            }
            sv_PopRequest = 1;
        }
    }
    d2_RadioClear(g);
    d2_CloseView(view);
    return r;
}
