/* 0x58af50, the choose-player window (2352 B, ROUGH SKELETON): reads the profile "Network" (the name into
   0x75e5aa, the family name into 0x75e5ca, the emblem 0x75e5a4 = the profile int + 0x59a, the gender word
   0x75e5a8), the window "Menu\nCHOOSEPLAYER" 0x6250d0 (0x4741f0 / 0x475600), the city name 0x5142e0
   (0x936476) and the controls 1 .. 6 (0x475b80, 0x473650 x2, 0x52e350 x2, 0x515960, 0x475cb0 x2); the eight
   emblem buttons 0x4731d0 ((i % n) * 0x30 + 0x50, (i / n) * 0x30 + 0x20, i + 0x59a) with their window entries
   (+0x48 = 1, +0x28 = 0, +0x2c = 0, +0x50 = 1, +0x1c0 = 3) and 0x46c560; the chosen emblem / gender
   highlighted (0x46c630); the state machine 0 .. 5 over the two text fields, the two gender buttons and the
   eight emblems; the frame loop 0x5309a0 (0x5361d0, mask) with the profile write back and the return 1 when a
   player was chosen. ROUGH: the state machine and the profile write back are condensed to the calls; see the
   disassembly 0x58af50 .. 0x58b8xx
   @deep Q1 (unit unnamed, @named_by -); B: code 745 + order 10 + fill 10 + slots 2, ROUGH/SKELETON draft of a 2352 B window; Q2 (17 callees), Q3 (25 addresses), Q4 U_I/U_B + cast; needs the choose-player window rebuilt against the original */

int ai_Unk58af50(int a, int b)
{
    char name[48];
    char buf[256];
    int btns[8];
    int win;
    int obj;
    int ret;
    int i;
    int id;
    int g1;
    int g2;
    int n;

    ret = 0;
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Name", (char *)(unsigned)"Vorname", (char *)((unsigned)&init_PlayerProfile + 0xa), 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Familienname", (char *)(unsigned)"Nachname", (char *)((unsigned)&init_PlayerProfile + 0x2a), 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    U_I(((unsigned)&init_PlayerProfile + 0x4)) = GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Wappen", 0, (char *)(unsigned)&ai_OptionsFile) + 0x59a;
    U_B(((unsigned)&init_PlayerProfile + 0x8)) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Geschlecht", 0, (char *)(unsigned)&ai_OptionsFile);
    U_B(((unsigned)&init_PlayerProfile + 0x9)) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Glauben", 0, (char *)(unsigned)&ai_OptionsFile);
    win = d2_OpenForm(0, 0, (char *)(unsigned)"Menu\\CHOOSEPLAYER");
    d2_CentreViewWindows(win);
    io_TownDisplayName((char *)(unsigned)&full_data_936476, buf);
    d2_SetForm(win, 1);
    io_PrintText(0x1acf, buf);
    n = d2_GetWinObj(win, 1, io_PrintText(0x1ad0, 0x1ad2));
    d2_SetForm(win, 2);
    g1 = d2_GetWinObj(win, 2, io_PrintText(0x1ad1, 0x1ad3));
    d2_SetForm(win, 3);
    io_PrintText(0x1ad4);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0x1c, (char *)&g2, 2, U_I((unsigned)&full_data_1440d14), U_I((unsigned)&full_data_1440d18));
    d2_SetForm(win, 4);
    io_PrintText(0x1ad7);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0x1c, (char *)&g2, 2, U_I((unsigned)&full_data_1440d20), U_I((unsigned)&full_data_1440d24));
    d2_SetForm(win, 5);
    io_PrintText(0x1adb);
    n = (*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) + -0x20) / 0x30;
    obj = d2_CreateRadioGroup(0);
    for (i = 0; i < 8; i++) {
        id = d2_WinOpenPic((i % n) * 0x30 + 0x50, (i / n) * 0x30 + 0x20, i + 0x59a, U_I((unsigned)&d2_LastWindow));
        btns[i] = id;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x48) = 1;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x28) = 0;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x2c) = 0;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x50) = 1;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1c0) = 3;
        d2_RadioAdd(obj, id);
    }
    d2_SetForm(win, 6);
    id = d2_GetWinObj(win, 6, io_PrintText(0x7e));
    d2_SetObjValue(n, (char *)((unsigned)&init_PlayerProfile + 0xa));
    d2_SetObjValue(g1, (char *)((unsigned)&init_PlayerProfile + 0x2a));
    for (i = 0; i < 8; i++)
        if (U_I(U_I((unsigned)&d2_Objs) + btns[i] * 0x2ec + 8) == U_I(((unsigned)&init_PlayerProfile + 0x4))) {
            d2_RadioSelect(obj, i);
            break;
        }
    d2_SetObjMax(n, 0xc);
    d2_SetObjMax(g1, 0xc);
    g2 = d2_CreateRadioGroup(2, g1, 0);
    d2_RadioSelect(g2, U_B(((unsigned)&init_PlayerProfile + 0x8)));
    g2 = d2_CreateRadioGroup(2, g1, 0);
    d2_RadioSelect(g2, U_B(((unsigned)&init_PlayerProfile + 0x9)));
    d2_ClearInput();
    U_B((unsigned)&kb_Last) = 0;
    /* ROUGH: the state machine 0 .. 5 (the two text fields 0x475980 -> 0x75e5aa / 0x75e5ca, the gender
       buttons 0x46c6c0 and the clicked 0x5fe710, the eight emblems, the exit) and the profile write back
       (WritePrivateProfileStringA x5, 0x5e56c4) are not written yet; the frame pump 0x5309a0 (0x5361d0, b) */
    while (1) {
        d2_MenuNavigate(obj);
        if (sv_RunFrame((unsigned)&main_Unk5361d0, b) == 0) {
            d2_RadioClear(g2);
            d2_RadioClear(g1);
            d2_RadioClear(obj);
            d2_CloseView(win);
            return ret;
        }
    }
}
