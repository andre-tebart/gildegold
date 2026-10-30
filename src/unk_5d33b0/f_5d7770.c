/* 0x5d7770, the options menu screen (1112 B): param none, void. The game screen 0x5d7160 is copied
   into the local 0x2580-dword image, the screen fades black (0x5d6fa0), the window "menu\options"
   0x62dbb8 (0x4741f0 / 0x475870 (1) / 0x475850 (2) / d2_SetForm (0)) with the screen 0x52ea30
   (0x6ed938) and 0x520f20 is shown and the seven buttons are built by 0x5d6f40 (the texts 0x1441330
   .. 0x1441348, the font 0x6ed938): ids 0 / 1 (load / save: 0x475e40 off in a network game), 2 (leave
   the game, confirmed in the 0x200-byte text buffer), 3 (graphics options 0x5d5fd0), 4 (sound options
   0x5d6580), 5 (game options 0x5d6950) and 7 (back), then the radio group 0x46c490 (7, id0, id1, id2,
   id3, id4, id5, id7), 0x423a50 (0) and the frame flags 0x6f0b60 saved / cleared, 0x4c3140. The pump
   0x5309a0 (0x5d7770, 0x24087) loops: d2_MenuNavigate (group); a pending screen 0x6f22a0 = 1 on
   0x14673a0 / 0x1468410; a click 0x5fe710: id0 -> 0x475590 (wnd, 0), 0x5d4080 () -> the game start
   (0x6f25a0 = 0x6f259c = 1, 0x52fb50 (0x53bb20)); id1 -> 0x5d4920 / 0x5d45d0 (the image); id5 ->
   0x5d6950; id2 -> the confirmation text 0x5176d0 (id 0x1ca4) + the box 0x5d3810 -> 0x4c3f90 /
   0x4c3ae0 wait, 0x6f259c = 1, 0x52fb50; id3 -> 0x5d5fd0; id4 -> 0x5d6580; id7 -> 0x6f22a0 = 1.
   The click chain order is id0, id1, id5, id2, id3, id4, id7 (the original's compare order, not the
   creation order). On a zero pump: 0x5d7080 (fade out), 0x6333e0 = 0, 0x6f25ae = 0, d2_RadioClear,
   d2_CloseView, the ambient volume restored from the sfx option byte, 0x6f0b60 restored, 0x4c30e0,
   and with the sign bit of the game flags set and no game end (0x6f259c) 0x576030 (tail call). */
#define U_F(a) (*(float *)(a))

/* the callees of the front-end stretch (their units' headers are not included here, so their
   prototypes are repeated as the definitions spell them; the building pointers are this TU's ints) */
int ai_StorageWindow(int building, int list); /* 0x5b26b0, unk_58bbd0.c */
int ai_ProductionWindow(unsigned char *building); /* 0x58bd60, unk_58bbd0.c: "Handel\Handel_PRODUKTION" */
int ai_OpenTransport(int rec);             /* 0x5af1a0, unk_58bbd0.c */
int ai_PersonnelBook(int building, char *text); /* 0x5aab80, unk_58bbd0.c: "ob_PERSONALBUCH" */
void ai_TradeSearchWindow(unsigned char *cfg, int arg); /* 0x595480, unk_58bbd0.c: "HANDEL\HANDEL_SUCHEN" */
void ai_MasterLetterWindow(unsigned char *building); /* 0x5c3200, unk_58bbd0.c: "ob_MEISTERBRIEF" */
void ai_MarketContact(struct AIBLD *rec);  /* 0x58f510, unk_58bbd0.c */
int sv_CheckMoney(char *o, int n);         /* 0x520f90, sv.c: 1 when the object's money (+0x194)
                                              covers n, else a message box with the text 0x74 */
int sv_MessageBox(int text, unsigned flags); /* 0x521150, sv.c: the modal message box */
void sv_CameraFollow(void);                /* 0x52d7a0, sv.c: the per-frame camera follow */
int ai_AmountMenu(int record, int text, int text2, int callback); /* 0x5c1130, unk_58bbd0.c: the
                                              "special_amt1" amount menu, returns the chosen value */
int d2_CreateWindow(); int d2_RemoveWindow(); int io_PrintText(char *fmt, ...);
int d2_GetObjValue();

void ai_OptionsMenu(void)
{
    int image[0x2580];
    char text[0x200];
    int wnd, group, pump;
    int id0, id1, id2, id3, id4, id5, id7;
    int saved;
    int click;

    ai_Screenshot();
    memcpy(image, ai_ScreenImage, sizeof(image));
    ai_LeaveGameScreen();
    wnd = d2_OpenForm(0, 0, "menu\\options");
    sv_SetScreenCenter(0x24087);
    d2_PlaceViewWindows(wnd, 1);
    d2_PlaceMainWindowF(wnd, 2);
    d2_SetForm(wnd, 0);
    sv_FitForm(d2_LastWindow);
    sv_ResetScreenClip();
    d2_SetForm(0, 0);
    id0 = ai_MenuButton(0, (int)ai_TextLoadGame, d2_LastWindow);
    id1 = ai_MenuButton(1, (int)ai_TextSaveGame, d2_LastWindow);
    if ((ai_GameFlags & 4) != 0) {
        d2_SetObjEnabled(id0, 0);
        if ((ai_GameFlags & 0x10) == 0)
            d2_SetObjEnabled(id1, 0);
    }
    if ((ai_GameFlags & 0x80) != 0) {
        unk_ClearEndGameGate();
        d2_SetObjEnabled(id0, 0);
        d2_SetObjEnabled(id1, 0);
    }
    sv_MenuOpen = 1;
    id2 = ai_MenuButton(2, (int)ai_TextGameOptions, d2_LastWindow);
    id3 = ai_MenuButton(3, (int)ai_TextGfxOptions, d2_LastWindow);
    id4 = ai_MenuButton(4, (int)ai_TextSfxOptions, d2_LastWindow);
    id5 = ai_MenuButton(5, (int)ai_TextLeaveGame, d2_LastWindow);
    if ((ai_GameFlags & 4) == 0)
        tb_Paused = 1;
    id7 = ai_MenuButton(7, (int)ai_TextBack, d2_LastWindow);
    group = d2_CreateRadioGroup(7, id0, id1, id2, id3, id4, id5, id7);
    msx_SetAmbVolume(0);
    saved = snd_AmbientOnByte;
    snd_AmbientOnByte = 0;
    snd_StopMarketAmbience();
    pump = sv_RunFrame(ai_OptionsMenu, 0x24087);
    while (pump != 0) {
        d2_MenuNavigate(group);
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0 && (click = d2_PointerObj) != -1) {
            if (click == id0) {
                d2_SetViewEnabled(wnd, 0);
                if (ai_LoadGameWindow() != 0) {
                    ai_LeaveGame = 1;
                    ai_GameOver = 1;
                    sv_PopTo((int)main_GameLoop);
                } else {
                    d2_SetViewEnabled(wnd, 1);
                }
            } else if (click == id1) {
                d2_SetViewEnabled(wnd, 0);
                if ((ai_GameFlags & 4) == 0)
                    ai_SaveOptionWindow(image);
                else
                    ai_SaveNameWindow(image);
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id5) {
                d2_SetViewEnabled(wnd, 0);
                txt_Format(text, 0x1ca4);       /* 0x1ca4: the "leave the game?" text of the text table */
                if (ai_MessageBox((int)text, 1) != 0) {
                    int r = cm_SendPlayerId();
                    if (r != -1) {
                        do {
                            if (cm_GetSlotState(r) != 0)
                                break;
                            sv_PumpNet();
                        } while (1);
                    }
                    ai_GameOver = 1;
                    sv_PopTo((int)main_GameLoop);
                } else {
                    d2_SetViewEnabled(wnd, 1);
                }
            } else if (click == id2) {
                d2_SetViewEnabled(wnd, 0);
                ai_OptionsGameScreen();
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id3) {
                d2_SetViewEnabled(wnd, 0);
                ai_OptionsGfxScreen();
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id4) {
                d2_SetViewEnabled(wnd, 0);
                ai_OptionsSfxScreen();
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id7) {
                sv_PopRequest = 1;
            }
        }
        pump = sv_RunFrame(ai_OptionsMenu, 0x24087);
    }
    ai_ResumeGameScreen();
    if ((ai_GameFlags & 4) == 0)
        tb_Paused = 0;
    sv_MenuOpen = 0;
    d2_RadioClear(group);
    d2_CloseView(wnd);
    msx_SetAmbVolume(ai_SoundOptions.sfxVolume);
    snd_AmbientOnByte = saved;
    snd_StartMarketAmbience();
    if ((ai_GameFlags & 0x80) != 0 && ai_GameOver == 0)
        unk_SetEndGameGate();
}
