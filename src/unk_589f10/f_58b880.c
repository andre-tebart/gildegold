/* 0x58b880, the choose-profession window (496 B): the window "Menu\nCHOOSEPROFESSION" 0x6250b8 (0x4741f0 /
   0x475600), control 0 and the text 0x1afa; the eleven profession buttons (the kinds 1 .. 7, 0xb, 0xd .. 0xf)
   at (i % 3) * 0x60 + 100, ((i / 3) * 5 + 5) * 0x10 through 0x4731d0 (the id i + 0x5a1), their window entry
   0xf8f080 + id * 0x2ec (+0x4c = 1, +0x1c0 = 3), the text +0xbc = the profession name 0x143a978[i], +0x1bc =
   0x56 and 0x46c560 (the object list); the frame loop 0x5309a0 (0x58b880, mask) with 0x46c6c0, the clicked
   control 0x5fe710 against the ids -> 0x5043d0 (the profession, 1) into 0x75e5a3, 0x6f22a0 = 1, return 1
   @deep Q1 (unit unnamed, @named_by -); B: code 70 + fill 3 + regs 2 (the choose-profession window); Q2 (12 callees), Q3 (8 addresses), Q4 U_I/U_B */

struct G18LOG * ai_Unk4cd480(); struct G18LOG * ai_Unk4cd4c0(); void d2_ScrollWindow();

int ai_Unk58b880(int param)
{
    int ids[22];
    int win;
    int obj;
    int ret;
    int i;
    int id;
    int n;

    ret = 0;
    ids[0] = 1;
    ids[1] = 2;
    ids[2] = 3;
    ids[3] = 4;
    ids[4] = 5;
    ids[5] = 6;
    ids[6] = 7;
    ids[7] = 0xb;
    ids[8] = 0xd;
    ids[9] = 0xe;
    ids[10] = 0xf;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_6250b8);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    io_PrintText(0x1afa);
    obj = d2_CreateRadioGroup(0);
    for (i = 0; i < 0xb; i++) {
        id = d2_WinOpenPic((i % 3) * 0x60 + 100, ((i / 3) * 5 + 5) * 0x10, ids[i] + 0x5a1, U_I((unsigned)&d2_LastWindow));
        ids[i + 0xb] = id;
        n = id * 0x2ec;
        U_I(n + 0x4c + U_I((unsigned)&d2_Objs)) = 1;
        U_I(n + 0x1c0 + U_I((unsigned)&d2_Objs)) = 3;
        strcpy((char *)(n + 0xbc + U_I((unsigned)&d2_Objs)), (char *)U_I(ids[i] * 4 + ((unsigned)&ai_Unk143a680 + 0x2f8)));
        U_I(n + 0x1bc + U_I((unsigned)&d2_Objs)) = 0x56;
        d2_RadioAdd(obj, id);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    n = sv_RunFrame((int)ai_Unk58b880, param);
    while (1) {
        if (n == 0) {
            d2_RadioClear(obj);
            d2_CloseView(win);
            return ret;
        }
        d2_MenuNavigate(obj);
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < 0xb; i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == ids[i + 0xb]) {
                    U_B(((unsigned)&init_PlayerProfile + 0x3)) = gm_TitleNumber(ids[i], 1);
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 1;
                    break;
                }
            }
        }
        n = sv_RunFrame((int)ai_Unk58b880, param);
    }
}
