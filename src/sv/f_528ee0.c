/* 0x528ee0, opens the default info panel when no panel is open (0x623860): without a selection the "standard" panel with the buttons 0x8c / 0xa6; for an own building (0x6f230c with the owner +0x27 = the player) the
   building panel with the name, the image, the health bar and the buttons 0x8c, 0xa6 (0x623898 for kind 0x1e) */

extern int ai_Unk623884, ai_Unk623898, ai_Unk623880, ai_Unk6f2334, ai_GameState;  /* 0x623884, 0x623898, 0x623880, 0x6f2334, 0x6f25ac */
extern char ai_Unk6f2338;  /* 0x6f2338 */
extern int ai_Unk5fe710;  /* 0x5fe710 */
extern char *ai_Unk7662c0p;
extern unsigned short * ai_Unk75ec5c;
extern int ai_Unk6f23bc, ai_Unk6f23c0, ai_Unk6f23c4, ai_Unk6f23c8, ai_Unk6f23cc, ai_Unk6f23d0, ai_Unk6f23d4, ai_Unk6f23d8, ai_Unk6f2294, ai_BuildingPerms, ai_Unk6f2320;  /* 0x6f23bc, 0x6f23c0, 0x6f23c4, 0x6f23c8, 0x6f23cc, 0x6f23d0, 0x6f23d4, 0x6f23d8, 0x6f2294, 0x6f231c, 0x6f2320 */
extern int ai_Unk5fe7d0;  /* 0x5fe7d0 */
int gm_Opinion(int a, int b);                     /* 0x50c520 */
int gm_MinutesBetween();                     /* 0x4fe280 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
void rt_Unk4bf5b0();                     /* 0x4bf5b0 */
int d2_RemoveWindowObjects();                     /* 0x472d20 */
int gm_ListAmount();                     /* 0x50aaf0 */
int ai_Unk5763c0();                     /* 0x5763c0 */
void ai_Unk576040(int a, int b, int c, int d);                     /* 0x576040 */
int ai_AmountMenu();                     /* 0x5c1130 */
int ai_TransportWindow();                     /* 0x5af1d0 */
int ai_Unk5b8fc0();                     /* 0x5b8fc0 */
int ai_Unk5b84f0();                     /* 0x5b84f0 */
void ai_TechTreeWindow(int param_1, int param_2);                     /* 0x591bf0 */
int sv_ReturnZero(void);
void sv_Unk528ee0(void);
void sv_Unk5290f0(unsigned short *p);
void sv_Unk529380(void);
void sv_Unk528860(unsigned char *o);
void sv_Unk528b80(unsigned short *p);
void sv_Unk528d90(unsigned short *p);

void sv_Unk528ee0(void)
{
    int hp;
    char text[128];

    if (sv_PanelForm != -1)
        return;
    if (ai_FallbackBuilding == 0) {
        if (ai_PlayerBuilding == 0) {
            sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_standard");
            d2_SetForm(sv_PanelForm, 0);
            ai_Unk623884 = sv_Unk528800(0, 0x8c, ai_Unk1441f68);
            ai_Unk623888 = sv_Unk528800(0, 0xa6, ai_Unk1441f7c);
            return;
        }
    } else if (*(short *)(ai_FallbackBuilding + 0x27) == (short)ai_PlayerId) {
        hp = gm_BuildingLevel(ai_FallbackBuilding);
        sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
        d2_SetForm(sv_PanelForm, 0);
        d2_SetForm(sv_PanelForm, 1);
        io_CharacterNameMarkup(*(unsigned short *)(ai_FallbackBuilding + 0x27), 2, 1, text);
        io_PrintText("$Z%s$A%s$A%s", text, *(unsigned char *)ai_FallbackBuilding * 0xe + 0x48e, ai_FallbackBuilding + 5);
        d2_SetForm(sv_PanelForm, 0);
        d2_WinOpenPic(((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) - 1, 0x4c, *(unsigned char *)ai_FallbackBuilding + 0x427, d2_LastWindow);
        ai_Unk623868 = d2_WinOpenSlider(0x15, 0x7a, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(ai_Unk623868, 0, 100, hp);
        ai_Unk623894 = sv_Unk528800(0, 0x8c, ai_Unk1441f6c);
        if (*(unsigned char *)ai_FallbackBuilding == 0x1e) {
            ai_Unk623898 = sv_Unk528800(0, 0xa6, ai_Unk1441f70);
            return;
        }
        ai_Unk623888 = sv_Unk528800(0, 0xa6, ai_Unk1441f7c);
    }
}
