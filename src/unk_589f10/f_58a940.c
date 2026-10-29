/* 0x58a940, adds the lobby's buttons (144 B): control 4 of the window 0x62aa88 and 0x52e350 (screen 0x6ed938,
   0, the id 0x62aa7c, 2, U_I (0x144abe8), U_I (0x144abec)), control 8 and 0x52e350 (.., the id 0x62aa84, 1,
   U_I (0x144abf0)); in the single-player mode 0x9364dc 0x7f the control 0x62aa80 is disabled (the window table
   0xf8f080 + id * 0x2ec, +0x3c = 1)
   @deep byte but not readable: Q1 (unit unnamed, @named_by -), Q2 (d2_SetForm control API, sv_Unk52e350), Q3 (10 lobby globals), Q4 U_I; the whole lobby/control API has to be named and the globals typed first; no Q6 fix exists */

void sv_Unk52e350(int f, int y, int *out, int n, ...); int sv_CreateFormSlider(); int d2_WinOpenPic(short x, short y, int pic, int win); int d2_CreateWinText();  /* @original: the call at 284 passes 1 argument to d2.h's 4-parameter prototype */
int d2_FindPic(); void ai_Unk4cd820(int v); int io_TownDisplayName(char *name, char *out); int d2_CreateRadioGroup(int n, ...);
void d2_RadioAdd(int g, int obj); void d2_RadioSelect(int g, int sel); void d2_RadioClear(int g); void d2_MenuNavigate(int g);
int ai_Unk5e56c4(); void d2_ClearInput(void); int ai_Unk5ea986();

void ai_Unk58a940(void)
{
    d2_SetForm(U_I((unsigned)&full_data_62aa88), 4);
    sv_Unk52e350(U_I((unsigned)&d2_LastWindow), 0, (char *)(unsigned)&full_data_62aa7c, 2, U_I((unsigned)&full_data_144abe8), U_I((unsigned)&full_data_144abec));
    d2_SetForm(U_I((unsigned)&full_data_62aa88), 8);
    sv_Unk52e350(U_I((unsigned)&d2_LastWindow), 0, (char *)(unsigned)&full_data_62aa84, 1, U_I((unsigned)&full_data_144abf0));
    if (U_I((unsigned)&ai_Unk9364dc) == 0x7f)
        U_I(U_I((unsigned)&d2_Objs) + 0x3c + U_I((unsigned)&full_data_62aa80) * 0x2ec) = 1;
}
