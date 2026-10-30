/* 0x59d5e0, the money lender's exchange window with the city list "..geldleihe_Wechsel" (2352 B, ROUGH: the
   two lists and the exchange are condensed): the 16 cities of 0x7e3f94 + i * 0x2f4 with their open flags
   (byte -0xa8) into the rows; the window 0x4741f0, the texts 0x1900, 0x1902 (the record +0x69), 0x1903 /
   0x1901 (the city of the carrier 0x53e400 (1, 0, 0x27), +0xb0), 0x190a (no money 0x509e30); the two lists
   0x59c120 (param_1 + 0x5d, 5 / 4) and 0x59c6e0, the buttons 0x475aa0 / 0x5e56c4, the panel 0x472760, the
   fee window 0x59d2d0 and the exchange 0x59c700; a click on a city row opens 0x59d410 (param_1, the goods of
   the selected list record +0x3 / +0x5, the city byte) and refreshes the lists (0x477530) */

int gm_Unk509e30(void *list, unsigned char p); char * he_FindEvent(int n, ...); char * he_NextEvent(void); int ai_AmountMenu();
int gm_BuildingClass(); unsigned unk_Unk503530(unsigned p1, unsigned char *b, unsigned short *o, int c); int sv_MessageBoxGreen(); 
void sv_Unk52adb0(void); void init_LoadObjectScene();
char d3_DeflateScene(unsigned int flags); char d3_InflateScene(unsigned int flags); void init_SetCharacterAnimations(unsigned char *b);
void init_ShowBuildingScene(unsigned char *b, int slot); void init_FinishBuildingScene(unsigned char *b, unsigned short *o); 
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); void sv_Unk52bb70(int id);
void unk_Unk5136b0(void); void snd_UnloadBuildingVoices(void); void unk_Unk513740(void); 
struct SBSAMPLE * sb_PlaySample(); void ai_ThievesGuildEmptyListener(); void ai_Unk5a4d40(); void ai_Unk5a4f70(); void ai_Unk5a5080();
void ai_Unk5a5bc0(); void ai_DungeonContact(); void ai_Unk5a3df0(); void ai_DungeonEmptyListener(); void ai_Unk5a3810(); void ai_ChurchContact();
void ai_Unk5a1a60(); void ai_Unk5a36c0(); void ai_Unk5a07d0(); void ai_Unk5a18d0();
void ai_Unk5a1960(); void ai_Unk5a6a90(); void ai_Unk5a67a0(); void ai_Unk5a6920();
void ai_Unk5821d0(char *rec, char *list); void ai_Unk582560(char *rec, char *list); void ai_Unk582600(char *rec, char *list); void ai_ProductionContact(int building, int arg);
void ai_Unk5a2de0(); void ai_Unk59fd30();
void ai_Unk59fca0();

void ai_Unk59d5e0(int param_1)
{
    int recsA[16 * 7];
    int recsB[16 * 7];
    int cities[16 * 7];
    char citybytes[0x1b0];
    char text[8];
    char msg[432];
    int win;
    int ctl[5];
    int i;
    int n;
    int m;
    int sel;
    int idx;
    int carrier;
    int v;
    int r;
    int *p;

    for (i = 0; i < 16; i++) {
        recsA[i * 7] = -1;
        recsA[i * 7 + 3] = 0;
        recsB[i * 7] = -1;
        recsB[i * 7 + 3] = 0;
        cities[i * 7] = -1;
        cities[i * 7 + 3] = 0;
        citybytes[i * 0x1c] = 0;
    }
    for (i = 0, p = (int *)(unsigned)&ai_Unk7e3f94; p < (int *)((unsigned)&gm_Offers + 0x44); p += 0x17a, i++) {
        if (*((char *)p - 0xa8) != 0) {
            citybytes[i * 0x1c] = (char)i;
            U_I(citybytes + i * 0x1c + 0xc) = *p;
        }
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_Wechsel");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x73b, 0x73f);
    ctl[2] = d2_GetWinObj(win, 0, 2);
    ctl[3] = d2_GetWinObj(win, 0, 3);
    ctl[1] = d2_GetWinObj(win, 0, 1);
    ctl[0] = d2_GetWinObj(win, 0, 0);
    ctl[4] = d2_GetWinObj(win, 0, 4);
    d2_SetForm(win, 0);
    /* ROUGH: the two header texts at the control rectangles (0x473650, +2 / +2) */
    d2_CreateWinText((short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[1] * 0x2ec + 0x10) - *(short *)((unsigned)&d2_LastWindowPtr + 4)) + 2,
                 (short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[1] * 0x2ec + 0x16) -
                         *(short *)((unsigned)&d2_LastWindowPtr + 6) + *(short *)(U_I((unsigned)&d2_Objs) + ctl[1] * 0x2ec + 0x12)) + 2,
                 (char *)(unsigned)&ch_EmptyName, U_I((unsigned)&d2_LastWindow));
    d2_CreateWinText((short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[2] * 0x2ec + 0x10) - *(short *)((unsigned)&d2_LastWindowPtr + 4)) + 2,
                 (short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[2] * 0x2ec + 0x16) -
                         *(short *)((unsigned)&d2_LastWindowPtr + 6) + *(short *)(U_I((unsigned)&d2_Objs) + ctl[2] * 0x2ec + 0x12)) + 2,
                 (char *)(unsigned)&ch_EmptyName, U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0xe;
    d2_SetForm(win, 3);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, &ctl[0], 2, U_I((unsigned)&full_data_14405d8), U_I((unsigned)&full_data_144a118));
    ai_Unk59c450((int)recsB, win, 4, 0);
    for (i = 0; i < 16; i++)
        if (recsB[i * 7] != -1)
            d2_SetObjEnabled(recsB[i * 7], 0);
    sel = -1;
    for (;;) {
        if (sv_RunFrame(ai_Unk59d5e0, 0x677c7) == 0) {
            d2_ClearPointerItems();
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
        carrier = he_FindEvent(1, 0, 0x27);
        while (carrier != 0 && U_I(carrier + 0xac) != U_I(param_1 + 1))
            carrier = he_NextEvent();
        d2_SetForm(win, 2);
        io_PrintText("$C");
        io_PrintText(0x1902, U_I(param_1 + 0x69));
        if (carrier == 0)
            io_PrintText(0x1903);
        else
            io_PrintText(0x1901, (char *)(unsigned)&ai_PlayerSlots + *(unsigned char *)(carrier + 0xb0) * 0x2f4);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        d2_ClearPointerItems();
        if (carrier == 0)
            sel = ai_Unk59c120(U_I(param_1 + 0x5d), recsA, win, 5, 1);
        idx = ai_Unk59c6b0(recsB);
        if (carrier == 0 && idx != -1 && sel != -1) {
            d2_ClearPointerItems();
            if (ai_Unk59d410(param_1, recsA[sel * 7 + 3], recsA[sel * 7 + 5], citybytes[idx * 0x1c]) != 0) {
                d2_EndInputEdit();
                for (i = 0; i < 16; i++)
                    if (recsA[i * 7] != -1)
                        d2_SetObjValue(recsA[i * 7], 0, 0, 0);
            }
        }
        for (i = 0; i < 16; i++)
            if (recsB[i * 7] != -1)
                d2_SetObjEnabled(recsB[i * 7], sel != -1);
        n = ai_UsedRecordCount(recsA);
        m = ai_UsedRecordCount(recsB);
        if (n < 5) {
            d2_EnableObject(ctl[1], 0);
            d2_EnableObject(ctl[0], 0);
            d2_EnableObject(ctl[2], 0);
        } else {
            d2_EnableObject(ctl[1], 1);
            d2_EnableObject(ctl[0], 1);
            d2_EnableObject(ctl[2], 1);
            _itoa(n, text, 10);
            d2_SetObjText(ctl[2], text);
        }
        if (m < 5) {
            d2_EnableObject(ctl[2], 0);
            d2_EnableObject(ctl[3], 0);
            d2_EnableObject(ctl[0], 0);
        } else {
            d2_EnableObject(ctl[2], 1);
            d2_EnableObject(ctl[3], 1);
            d2_EnableObject(ctl[0], 1);
            _itoa(m, text, 10);
            d2_SetObjText(ctl[0], text);
        }
        if (U_I((unsigned)&d2_ClickedId) == 3 || U_I((unsigned)&d2_ClickedId) == 4) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl[1]) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, -0x4c, U_I((unsigned)&d2_LastWindow));
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[3]) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, -0x4c, U_I((unsigned)&d2_LastWindow));
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[2]) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, 0x4c, U_I((unsigned)&d2_LastWindow));
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[0]) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, 0x4c, U_I((unsigned)&d2_LastWindow));
            }
        }
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl[1])
                ai_Unk59d2d0(param_1);
            else if (U_I((unsigned)&ai_Unk5fe710) == ctl[3])
                ai_Unk59c700(param_1);
        }
    }
}
