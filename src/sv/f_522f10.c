/* 0x522f10, the city / time panel (the big panel at the top of the screen): a = 1 builds the widgets (the city name, the shield, the date, the clock, the name plates), 0 refreshes
   their texts and the sky clock / sun hand; ROUGH (the widget arguments are read from the disassembly, the layout of the texts is guessed) */

/* ---- layout 2026-10-23 (session 5): drafts of the functions after 0x52a050 (ROUGH: absolute-address macros for the globals not declared above) ---- */
#define SV_I(a) (*(int *)(a))
#define SV_U(a) (*(unsigned *)(a))
#define SV_S(a) (*(short *)(a))
#define SV_US(a) (*(unsigned short *)(a))
#define SV_B(a) (*(unsigned char *)(a))
#define SV_C(a) (*(char **)(a))
#define SV_W(i) (d2_Objs + (i) * 0x2ec)            /* widget i */
#define SV_WI(i, o) (*(int *)(SV_W(i) + (o)))
#define SV_WS(i, o) (*(short *)(SV_W(i) + (o)))
#define SV_WB(i, o) (*(char *)(SV_W(i) + (o)))

void d2_RemoveObj();                     /* 0x46e710 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */
int d2_CreateWinText(short x, short y, char *text, int win);                     /* 0x473650 */
int d2_CreateScreenText(short x, short y, char *text);                     /* 0x473860 */
int d2_CreatePicObj();                     /* 0x46cd80 */
void d2_MoveObj(short x, short y, int obj);                     /* 0x46cda0 */
void d2_DrawText(int x, int y, void *surf, char *text, unsigned char flags);                     /* 0x46f000 */
void d2_DrawTextBox(int x, int y, int w, int h, void *surf, char *text, unsigned int flags);                     /* 0x46f1a0 */
void d2_AddDirty();                     /* 0x468eb0 */
int d2_FindPic();                     /* 0x468890 */
int d2_ShpBlit();                     /* 0x47bd70 */
void d2_SetClipRect(int x0, int y0, int x1, int y1);                     /* 0x47cb40 */
void * d2_SetFont(int font);                     /* 0x476030 */
void d2_EnableObject();                     /* 0x475aa0 */
int d2_SetForm(int view, int n);                     /* 0x475f70 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */
int gm_Season();                     /* 0x4fe330 */
int gm_ClockSeason(void *t);           /* 0x4fe350, f3gm.c */                     /* 0x4fe350 */
int ls_GetLanguage();                     /* 0x5190a0 */
char * gm_PlayerSeat(int p);                     /* 0x504a80 */
int gm_Unk509e30(void *list, unsigned char p);                     /* 0x509e30 */
int gm_ConvertMoney(int a, unsigned char k);                     /* 0x5085b0 */
void txt_Format(char *out, int id0, ...);  /* 0x5176d0 */
char *_strupr(char *);                     /* 0x5ea986 */

void sv_Unk522f10(unsigned char a, int b)
{
    struct { short s[2]; } t;
    char buf[0x100], name[0x200];
    char *p;
    int h, n, who;

    if (SV_B(((unsigned)&ai_Chars + 0x2) + ai_PlayerId * 0x21c) != 6)
        return;
    /* 0x522f42: the grabbed view shape back onto the screen at the view rectangle. The skeleton called
       d2_ShpBlit(0, 0, 0), so the shape was whatever the stack held (a click on "Weiter" then crashed in
       d2_ShpBlit with esi = 1); the original pushes sv_ViewRectLeft, sv_ViewRectTop, d2_ScreenSurf,
       ai_Unk6f22c0. */
    if (SV_I((unsigned)&ai_Unk6f22c0) != 0)
        d2_ShpBlit(sv_ViewRectLeft, sv_ViewRectTop, (struct D2SURF *)d2_ScreenSurf, (struct D2SHP *)ai_Unk6f22c0);
    if (a & 1) {
        if (ai_Unk623820 != -1) { d2_RemoveObj(ai_Unk623820); ai_Unk623820 = -1; }
        if (ai_Unk623824 != -1) { d2_RemoveObj(ai_Unk623824); ai_Unk623824 = -1; }
        if (ai_Unk623828 != -1) { d2_RemoveObj(ai_Unk623828); ai_Unk623828 = -1; }
        if (ai_Unk623848 != -1) { d2_RemoveObj(ai_Unk623848); ai_Unk623848 = -1; }
        if (ai_Unk62384c != -1) { d2_RemoveObj(ai_Unk62384c); ai_Unk62384c = -1; }
        if (ai_Unk62382c != -1) { d2_RemoveObj(ai_Unk62382c); ai_Unk62382c = -1; }
        if (ai_Unk623830 != -1) { d2_RemoveObj(ai_Unk623830); ai_Unk623830 = -1; }
        if (ai_Unk623834 != -1) { d2_RemoveObj(ai_Unk623834); ai_Unk623834 = -1; }
        if (ai_Unk623838 != -1) { d2_RemoveObj(ai_Unk623838); ai_Unk623838 = -1; }
        if (ai_Unk62383c != -1) { d2_RemoveObj(ai_Unk62383c); ai_Unk62383c = -1; }
        ai_Unk623820 = d2_WinOpenPic(0x10, 8, SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c), ai_Unk623840);
        SV_WI(ai_Unk623820, 0x1c0) = 0x43;
        SV_WI(ai_Unk623820, 0x4c) = 1;
        ai_Unk623824 = d2_WinOpenPic(0x23a, 0xd, 0x522, ai_Unk623840);
        SV_WI(ai_Unk623824, 0x1c0) = 0x43;
        SV_WI(ai_Unk623824, 0x4c) = 1;
        ai_Unk623848 = d2_CreateWinText(0x39, 0xd, ch_EmptyName, ai_Unk623840);
        SV_WI(ai_Unk623848, 0x1c0) = 0x43;
        SV_WI(ai_Unk623848, 0x4c) = 1;
        SV_WS(ai_Unk623848, 0x74) = 0x55;
        ai_Unk623830 = d2_CreatePicObj(d2_ScreenW - 0x8d, d2_ScreenH - 0x73, 0x583);
        SV_WI(ai_Unk623830, 0x1c0) = 0x43;
        SV_WI(ai_Unk623830, 0x4c) = 1;
        ai_Unk623834 = d2_CreatePicObj(d2_ScreenW - 0x57, d2_ScreenH - 0x4f, 0x7d9);
        SV_WI(ai_Unk623834, 0x1c0) = 3;
        SV_WI(ai_Unk623834, 0x4c) = 1;
        ai_Unk62382c = d2_CreatePicObj(d2_ScreenW - 0x2a, d2_ScreenH - 0x66, 0x584);
        SV_WI(ai_Unk62382c, 0x1c0) = 0x43;
        SV_WI(ai_Unk62382c, 0x4c) = 1;
        ai_Unk623838 = d2_CreatePicObj(d2_ScreenW - 0x4a, 0xd6, 0x585);
        SV_WI(ai_Unk623838, 0x1c0) = 0x43;
        SV_WI(ai_Unk623838, 0x4c) = 1;
        ai_Unk62383c = d2_CreatePicObj(d2_ScreenW - 0x48, 0xd7, 0x551);
        SV_WI(ai_Unk62383c, 0x1c0) = 0x43;
        SV_WI(ai_Unk62383c, 0x4c) = 1;
        SV_WI(ai_Unk62383c, 0x18) = 8;
        ai_Unk62384c = d2_CreateScreenText(d2_ScreenW - 0x6a, d2_ScreenH - 0x7e, ch_EmptyName);
        SV_WI(ai_Unk62384c, 0x5c) = 1;
        SV_WS(ai_Unk62384c, 0x74) = 0x54;
        SV_WS(ai_Unk62384c, 0x14) = 0x6b;
        SV_WS(ai_Unk62384c, 0x16) = 0x20;
    } else {
        /* the texts of the six plates come from the text lists 0x1441ec8 .. 0x1441ee0 (or are cleared) */
        if (SV_B((unsigned)&ai_OptionPanelHelp) == 0) {
            if (ai_Unk623820 != -1) SV_WB(ai_Unk623820, 0xbc) = ch_EmptyName[0];
            if (ai_Unk623824 != -1) SV_WB(ai_Unk623824, 0xbc) = ch_EmptyName[0];
            if (ai_Unk623848 != -1) SV_WB(ai_Unk623848, 0xbc) = ch_EmptyName[0];
            if (ai_Unk623830 != -1) SV_WB(ai_Unk623830, 0xbc) = ch_EmptyName[0];
            if (ai_Unk62382c != -1) SV_WB(ai_Unk62382c, 0xbc) = ch_EmptyName[0];
            if (ai_Unk62383c != -1) SV_WB(ai_Unk62383c, 0xbc) = ch_EmptyName[0];
        } else {
            if (ai_Unk623820 != -1) strcpy(SV_W(ai_Unk623820) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d4)));
            if (ai_Unk623824 != -1) strcpy(SV_W(ai_Unk623824) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9e0)));
            if (ai_Unk623848 != -1) strcpy(SV_W(ai_Unk623848) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d0)));
            if (ai_Unk623830 != -1) strcpy(SV_W(ai_Unk623830) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9cc)));
            if (ai_Unk62382c != -1) strcpy(SV_W(ai_Unk62382c) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9c8)));
            if (ai_Unk62383c != -1) strcpy(SV_W(ai_Unk62383c) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d8)));
        }
        sv_Unk5229b0(a);
        d2_SetFont(0x55);
        gm_ClockToDate((unsigned)&ai_Time, &t);
        if (ls_GetLanguage() == 2) {
            n = SV_I(((unsigned)&d2_Strings + 0x130) + gm_Season(&t, t.s[1]) * 4);
            sprintf(buf, "%s A.D.%i", n);
            d2_DrawText(SV_S(((unsigned)&d2_Wins + 0x4) + ai_Unk623840 * 0x3c4) + 0x181, 0x17, d2_ScreenSurf, buf, 0);
        } else {
            n = SV_I(((unsigned)&d2_Strings + 0x130) + gm_Season(&t, t.s[1]) * 4);
            sprintf(buf, "%s A.D.%i", n);
            d2_DrawText(SV_S(((unsigned)&d2_Wins + 0x4) + ai_Unk623840 * 0x3c4) + 0x195, 0x17, d2_ScreenSurf, buf, 0);
        }
        h = (int)((SV_I(((unsigned)&ai_Time + 0x6)) % 0x3c) * 0.8);
        d2_DrawShape(d2_ScreenW - 0x50, 0x11, d2_ScreenSurf, d2_GetPic(0x581), h);
        d2_AddDirty(d2_ScreenW - 0x50, 0x11, SV_S((unsigned)&d2_ShpW), SV_S((unsigned)&d2_ShpH));
        h = (int)((SV_US((unsigned)&ai_TimeHour) * 0x3c + SV_I(((unsigned)&ai_Time + 0x6))) * 4.0 * 0.016666666666666666) % 0x30;
        d2_DrawShape(d2_ScreenW - 0x50, 0x11, d2_ScreenSurf, d2_GetPic(0x582), h);
        d2_AddDirty(d2_ScreenW - 0x50, 0x11, SV_S((unsigned)&d2_ShpW), SV_S((unsigned)&d2_ShpH));
        if (SV_I((unsigned)&d2_HitUnderPointer) == ai_Unk623850) {
            sprintf(buf, "%02i:%02i", SV_US((unsigned)&ai_TimeHour), SV_I(((unsigned)&ai_Time + 0x6)));
            d2_SetFont(0x54);
            d2_DrawTextBox(d2_ScreenW - 0x50, 2, 0x2e, 0x14, d2_ScreenSurf, buf, 0x28);
            d2_AddDirty(d2_ScreenW - 0x50, 2, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
            d2_SetFont(0x55);
        }
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&ai_PlayerBuilding) == 0 && ai_Unk62383c != -1) {
            if (ai_Unk623838 != -1) {
                d2_EnableObject(ai_Unk623838, 0);
                d2_EnableObject(ai_Unk62383c, 1);
                h = (int)(*(float *)(SV_I((unsigned)&ai_Unkfc2dec) + 0x88) * 6.2831853 * 0.15915494 * 48.0 + 48.5);
                SV_WI(ai_Unk62383c, 0x78) = h % 0x30;
            } else {
                d2_EnableObject(ai_Unk62383c, 0);
            }
        } else {
            if (ai_Unk623838 != -1)
                d2_EnableObject(ai_Unk623838, 1);
            if (ai_Unk62383c != -1)
                d2_EnableObject(ai_Unk62383c, 0);
        }
        d2_SetFont(0x55);
        if (ai_Unk623848 != -1) {
            io_CharacterName(ai_PlayerId, 1, 2, name);
            strcpy(buf, name);
            d2_SetObjText(ai_Unk623848, buf);
        }
        sprintf(buf, "%i", SV_I(((unsigned)&ai_Selected + 0xc) + ai_PlayerId * 0x21c));
        d2_DrawText(SV_S(((unsigned)&d2_Wins + 0x4) + ai_Unk623840 * 0x3c4) + 0x23a, 0x17, d2_ScreenSurf, buf, 0);
        if (ai_Unk623824 != -1)
            d2_MoveObj(SV_S(((unsigned)&d2_Wins + 0x4) + ai_Unk623840 * 0x3c4) + SV_S((unsigned)&d2_TextDrawnW) + 0x23d, 0x18, ai_Unk623824);
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0) {
            if (ai_Unk623828 == -1) {
                strcpy(name, (char *)(unsigned)&ai_PlayerSlots + SV_B((unsigned)&ai_OwnPlayer) * 0x2f4);
                _strupr(name);
                sprintf(buf, "_STADTWAPPEN_%s", name);
                p = (char *)d2_FindPic(buf);
                if (p != 0)
                    ai_Unk623828 = d2_CreatePicObj(d2_ScreenW - 0x6e, 0x5b, p);
                else
                    ai_Unk623828 = d2_CreatePicObj(d2_ScreenW - 0x6e, 0x5b, 0x5b1);
                SV_WI(ai_Unk623828, 0x1c0) = 0x43;
                SV_WI(ai_Unk623828, 0x4c) = 1;
            }
            if (SV_B((unsigned)&ai_OptionPanelHelp) == 0) {
                if (ai_Unk623828 != -1) SV_WB(ai_Unk623828, 0xbc) = ch_EmptyName[0];
            } else if (ai_Unk623828 != -1)
                strcpy(SV_W(ai_Unk623828) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9dc)));
            d2_SetFont(0x56);
            strcpy(name, (char *)((unsigned)&ai_PlayerSlots + 0x20) + SV_B((unsigned)&ai_OwnPlayer) * 0x2f4);
            d2_DrawTextBox(d2_ScreenW - 0x70, 0x5d, 0x68, 0x14, d2_ScreenSurf, name, 8);
            d2_AddDirty(d2_ScreenW - 0x70, 0x5d, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        } else if (ai_Unk623828 != -1) {
            d2_RemoveObj(ai_Unk623828);
            ai_Unk623828 = -1;
        }
        d2_SetFont(0x54);
        if (SV_I((unsigned)&sv_ResearchClass) == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_B(SV_I((unsigned)&ai_FallbackBuilding)) == 0x47)
                who = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x65);
            else
                who = SV_B((unsigned)&ai_OwnPlayer);
            n = gm_SumSlotAmounts(SV_I(((unsigned)&ai_Unk770cec + 0xc) + ai_PlayerId * 0x21c), who) + SV_I((unsigned)&full_data_6f22dc);
        } else {
            n = ((int (*)(void *))gm_Unk509e30)(*(int *)(gm_PlayerSeat(SV_I((unsigned)&sv_ResearchClass)) + 0x14)) + SV_I((unsigned)&full_data_6f22dc);
            who = SV_B((unsigned)&ai_OwnPlayer);
        }
        if (ai_Unk62384c != -1) {
            if (n != 0) {
                h = gm_ConvertMoney(n, who);
                txt_Format(buf, "%I~ %w", (int)(float)h, who);   /* 0x523d17: the second argument (esi) is
                                                                    the same owner index, for the %w city name */
                d2_SetObjText(ai_Unk62384c, buf);
            } else
                d2_SetObjText(ai_Unk62384c, SV_C(((unsigned)&ai_MessageFilterTexts + 0x8c0)));
        }
    }
    if (ai_Unk623820 != -1 && SV_WI(ai_Unk623820, 8) != SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c)) {
        d2_RemoveObj(ai_Unk623820);
        ai_Unk623820 = -1;
        ai_Unk623820 = d2_WinOpenPic(0x10, 8, SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c), ai_Unk623840);
        SV_WI(ai_Unk623820, 0x1c0) = 3;
        SV_WI(ai_Unk623820, 0x4c) = 1;
    }
    if (SV_I((unsigned)&full_data_62571c) >= 0) {
        if (SV_I((unsigned)&full_data_625718) > 0) {
            d2_EnableObject(ai_Unk6238a4, 1);
        } else {
            d2_EnableObject(ai_Unk6238a4, 0);
            SV_I((unsigned)&full_data_6f2364) = 0;
        }
        if (d2_ClickedId != -1 && ai_Unk5fe710 == ai_Unk6238a4)
            SV_I((unsigned)&full_data_6f2364) = SV_I((unsigned)&full_data_6f2364) == 0;
        if (SV_I((unsigned)&full_data_623e90) != SV_I((unsigned)&full_data_625718) || (d2_ClickedId != -1 && ai_Unk5fe710 == ai_Unk6238a4)) {
            if (SV_I((unsigned)&full_data_6f2364) != 0) {
                d2_SetForm(SV_I((unsigned)&full_data_6faa60), 0);
                d2_MoveObj(SV_S(((unsigned)&d2_Wins + 0x6) + d2_LastWindow * 0x3c4), d2_ScreenW - 0x1d - (SV_I((unsigned)&full_data_625718) << 5), SV_I(((unsigned)&ai_Unkf8f13c + 0x250) + d2_LastWindow * 0x3c4));
                d2_MoveObj(d2_ScreenW - (SV_I((unsigned)&full_data_625718) << 5) - 0x4d, d2_ScreenH - 0x21, ai_Unk6238a4);
            } else {
                d2_SetForm(SV_I((unsigned)&full_data_6faa60), 0);
                d2_MoveObj(d2_ScreenW - 0x7d, SV_S(((unsigned)&d2_Wins + 0x6) + d2_LastWindow * 0x3c4), SV_I(((unsigned)&ai_Unkf8f13c + 0x250) + d2_LastWindow * 0x3c4));
                d2_MoveObj(d2_ScreenW - 0xad, d2_ScreenH - 0x21, ai_Unk6238a4);
            }
            SV_I((unsigned)&full_data_623e90) = SV_I((unsigned)&full_data_625718);
        }
        if (SV_I((unsigned)&full_data_6f2364) != 0) {
            d2_SetClipRect(0, 0, d2_ScreenW - 0x8c, d2_ScreenH);
            d2_DrawShape(d2_ScreenW - (SV_I((unsigned)&full_data_625718) << 5) - 0x37, d2_ScreenH - 0x37, d2_ScreenSurf, d2_GetPic(sv_PanelPic + 3), 0);
            d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        }
    }
}
