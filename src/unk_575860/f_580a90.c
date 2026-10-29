/* 0x580a90, builds the cannon control panel (1042 B, ROUGH: all the calls are written; the screen layout value
   of 0x46cda0 (the CONCAT22 in the pseudocode) is a guess from the short 0xf8f124 + 0x6ed938 * 0x3c4): sc = the
   scene struct. The bottom / top windows "Locations_domizil_cannon_bottom" (0x62a3cc) and "_top" (0x62a3ac)
   with 0x475850 (1) / 0x475f70, the "res" window (0x62a38c) with 0x475850 (3) / 0x475590 (0); the two window
   bases 0x46cd80 (the ids 0x7cb / 0x7cc / 0x7cd by the width 800 / 0x400 / other and the height, 0x6ed918 +
   0x52 + the id * 0x54) into sc + 0xd4 / 0xd8; the four buttons 0x4731d0 (0x59 / 0x9b / 0x14 / 0x56 / 0x98, the
   texts 0x7d5 / 0x7d4 / 0x7d3 / 0x7d1 / 0x7d2) into sc + 0xb8 / 0xbc / 0xc8 / 0xcc / 0xd0 with the window table
   0xf8f080 +0x4c = 1 and +0x1c0 = 3; the two sliders 0x46a960 (4 / 100, the values 0x925114 / 0x925118) into
   sc + 0xb0 / 0xb4 with 0x46bc70 / 0x46bca0; the three bars 0x46a960 (0x7d / 0x4c2) into sc + 0xa8 / 0xac /
   0xc4 (0x5dbc20 and two __ftol 0x5de518 values) with 0x46bc70; sc + 0x70 = the clock 0x6333cc */

int d2_CreatePicObj(); void d2_MoveObj(short x, short y, int obj); void d2_PlaceMainWindow(); int d2_WinOpenPic(short x, short y, int pic, int win);
int d2_WinOpenSlider(); void d2_SetLabel(); void d2_SetList(int id, int n, ...);

void ai_Unk580a90(char *sc)
{
    int x;
    int mid;
    int id;
    int v;
    int w;

    x = U_I((unsigned)&d2_ScreenH);
    mid = (x + ((int)U_I((unsigned)&d2_ScreenW) >> 4) * -9) >> 1;
    if (U_I((unsigned)&d2_ScreenW) == 800) {
        v = 0x7cb;
        w = 0x7ce;
    } else if (U_I((unsigned)&d2_ScreenW) == 0x400) {
        v = 0x7cc;
        w = 1999;
    } else {
        v = 0x7cd;
        w = 2000;
    }
    v = d2_CreatePicObj(0, 0, v);
    *(int *)(sc + 0xd4) = v;
    v = d2_CreatePicObj(0, U_I((unsigned)&d2_ScreenH) - *(short *)((unsigned)&d2_Pics + 0x52 + w * 0x54), w);
    *(int *)(sc + 0xd8) = v;
    v = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3cc);
    *(int *)(sc + 0xa4) = v;
    d2_PlaceMainWindow(v, 1);
    d2_SetForm(*(int *)(sc + 0xa4), 0);
    d2_MoveObj(*(unsigned short *)(((unsigned)&d2_Wins + 0x4) + U_I((unsigned)&d2_LastWindow) * 0x3c4), (x - mid) - 0x19, *(int *)(((unsigned)&ai_Unkf8f13c + 0x250) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
    d2_SetForm(*(int *)(sc + 0xa4), 2);
    id = d2_WinOpenPic(0x59, 0xf, 0x7d5, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xb8) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xb8) * 0x2ec) = 3;
    id = d2_WinOpenPic(0x9b, 0xf, 0x7d4, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xbc) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xbc) * 0x2ec) = 3;
    d2_SetForm(*(int *)(sc + 0xa4), 1);
    v = d2_WinOpenSlider(0x140, 0x14, 100, 0, 100, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xb0) = v;
    d2_SetObjValue(v, 0, 4, U_I((unsigned)&ai_Unk925114), 1);
    d2_SetLabel(*(int *)(sc + 0xb0), U_I((unsigned)&full_data_144a344));
    d2_SetList(*(int *)(sc + 0xb0), 5, U_I((unsigned)&full_data_144a34c), U_I((unsigned)&full_data_144a350), U_I((unsigned)&full_data_144a354), U_I((unsigned)&full_data_144a358), U_I((unsigned)&full_data_144a35c));
    v = d2_WinOpenSlider(0x140, 0x32, 100, 0, 100, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xb4) = v;
    d2_SetObjValue(v, 0, 100, U_I((unsigned)&ai_Unk925118), 1);
    d2_SetLabel(*(int *)(sc + 0xb4), U_I((unsigned)&full_data_144a348));
    id = d2_WinOpenPic(0x14, 0x17, 0x7d3, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xc8) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xc8) * 0x2ec) = 3;
    id = d2_WinOpenPic(0x56, 0x17, 0x7d1, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xcc) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xcc) * 0x2ec) = 3;
    id = d2_WinOpenPic(0x98, 0x17, 0x7d2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xd0) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xd0) * 0x2ec) = 3;
    v = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3ac);
    *(int *)(sc + 0xc0) = v;
    d2_PlaceMainWindow(v, 1);
    d2_SetForm(*(int *)(sc + 0xc0), 0);
    d2_MoveObj(*(unsigned short *)(((unsigned)&d2_Wins + 0x4) + U_I((unsigned)&d2_LastWindow) * 0x3c4), mid - 0x4b, *(int *)(((unsigned)&ai_Unkf8f13c + 0x250) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
    d2_SetForm(*(int *)(sc + 0xc0), 1);
    v = d2_WinOpenSlider(0x96, 0xb, 200, 0, 100, 0x75, 0x4c2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xa8) = v;
    d2_SetObjValue(v, 0, 100, (int)ai_GetDefaultEffectScale(1));
    d2_SetLabel(*(int *)(sc + 0xa8), U_I((unsigned)&full_data_144a338));
    v = d2_WinOpenSlider(0x96, 0x1f, 200, 0, 100, 0x75, 0x4c2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xac) = v;
    d2_SetObjValue(v, 0, 100, (int)ai_GetDefaultEffectScale(1));
    d2_SetLabel(*(int *)(sc + 0xac), U_I((unsigned)&full_data_144a33c));
    v = d2_WinOpenSlider(0x96, 0x33, 200, 0, 100, 0x75, 0x4c2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xc4) = v;
    d2_SetObjValue(v, 0, 100, (int)ai_Unk5de518());
    d2_SetLabel(*(int *)(sc + 0xc4), U_I((unsigned)&full_data_144a340));
    v = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a38c);
    *(int *)(sc + 0x74) = v;
    d2_PlaceMainWindow(v, 3);
    d2_SetViewEnabled(*(int *)(sc + 0x74), 0);
    *(int *)(sc + 0x70) = U_I((unsigned)&ai_Unk6333cc);
}
