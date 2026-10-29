/* 0x525220, opens the player bar ("Misc\\PlayerBar" / "Misc\\PlayerBar_1" by 0x6f2f94) of the kind k (0x6f2368), the form 0x62385c at the screen bottom, its two sliders +0x1c0 = 3, hides the panel 0x623860, clears the 0x20
   member rows of 0x75f280 and creates the button 0x51d; the characters' flags (0x770d08) are cleared for the rows' owners. ROUGH */

extern char ai_GfxResolution;               /* 0x6f2f94, the player bar variant */
extern int ai_Unk760610, ai_Unk760480, ai_Unk760484, ai_Unk760548, ai_Unk76054c, ai_Unk760614, ai_Unk760618, ai_Unk76061c, ai_Unk760464, ai_Unk6f2370;  /* 0x760610, 0x760480, 0x760484, 0x760548, 0x76054c, 0x760614, 0x760618, 0x76061c, 0x760464, 0x6f2370 */
extern int ai_Unk760628, ai_Unk760620, ai_Unk760624, ai_Unk633dd4, ai_Unk75ec20, d3t_CamMove, ai_Unk623e80, ai_Unk6f236c;  /* 0x760628, 0x760620, 0x760624, 0x633dd4, 0x75ec20, 0x6310e8, 0x623e80, 0x6f236c */
extern char ai_Unk6f2340;
extern float ai_Unk760420, ai_Unk623e94;  /* 0x760420, 0x623e94 */
extern unsigned ai_Unk760488[0x18];
extern unsigned ai_Unk7604a0[0x18];     /* 0x7604a0 */
extern float ai_Unk760550[0x18], ai_Unk7605b0[0x18];  /* 0x760550, 0x7605b0 */
/* ai_MapRefs: view of d3_Scenes (tools/data_views.py) */
/* ai_Unk623baa: view of ai_Unk623ba9 (tools/data_views.py) */
const int ai_Unk5eeab8[24] = { 390, 510, 630, 1050, 1170, 1290, 300, 420, 540, 1140, 1260, 1380, 420, 540, 660, 1020, 1140, 1260, 480, 600, 690, 960, 1020, 1140 };  /* 0x5eeab8 */

extern char *ai_Unk5fb5b0;              /* 0x5fb5b0 */
extern int ai_Unk623cc0[], ai_Unk623e10[];  /* 0x623cc0, 0x623e10 */
extern int ai_Unkfba9c0[], ai_Unk5fcc78, ai_Unk5fcc7c, ai_Unk5fcc80;  /* 0xfba9c0, 0x5fcc78, 0x5fcc7c, 0x5fcc80 */
int d2_GetWinObj();                     /* 0x475b80 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
void d2_ClearPointerItems(void);                     /* 0x476c70 */
void sv_UpdatePanel(void);                     /* 0x52a050 */
int d2_WinOpenPicStyled();                     /* 0x473350 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */
int ai_Unk5dbc30();                     /* 0x5dbc30 */
int d3t_FreeSnow();                     /* 0x414b50 */
struct D3TRAIN * d3t_CreateRain();                     /* 0x413b30 */
int d3t_SetRain();                     /* 0x413d30 */
void d3t_DeleteRain();                     /* 0x413d00 */
int d3t_CreateSnow();                     /* 0x4148f0 */
int d3t_SetSnow();                     /* 0x414c60 */
int d3sky_Create();                     /* 0x45da30 */
int d3sky_DeleteLayer();                     /* 0x45d800 */
int d3sky_CreateLayer();                     /* 0x45d880 */
void d3sky_SetLayerSpeed(struct D3SKY *s, struct D3SKYLAYER *l, float speed);  /* 0x45d960 */
char d3_SetFogSet(unsigned int a, unsigned int b, float t);                     /* 0x44a170 */
char d3_SetLightSet(unsigned int set, float t, float scale, char calc);                     /* 0x449de0 */
void sv_UpdateSkyColors(void);
void sv_Unk525ad0(char *t);

void sv_OpenPlayerBar(int a, char k)
{
    int s, f, i;
    int *p;
    char *c;

    if (sv_PlayerBarForm != -1)
        return;
    ai_Unk6f2368 = k;
    sv_PlayerBarForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, ai_GfxResolution == 0 ? "Misc\\PlayerBar" : "Misc\\PlayerBar_1");
    d2_SetForm(sv_PlayerBarForm, 1);
    s = d2_LastWindow;
    d2_SetForm(sv_PlayerBarForm, 2);
    f = d2_GetWinObj(sv_PlayerBarForm, 2, 0);
    *(int *)(d2_Objs + 0x1c0 + f * 0x2ec) = 3;
    *(int *)(ai_Unkf8f128 + 0x3a0 + d2_LastWindow * 0x3c4) = f;
    *(int *)(d2_Objs + 0x1e0 + f * 0x2ec) = s;
    f = d2_GetWinObj(sv_PlayerBarForm, 2, 1);
    *(int *)(d2_Objs + 0x1c0 + f * 0x2ec) = 3;
    *(int *)(ai_Unkf8f128 + 0x3a4 + d2_LastWindow * 0x3c4) = f;
    *(int *)(d2_Objs + 0x1e0 + f * 0x2ec) = s;
    *(int *)(ai_Unkf8f128 + 0x25c + s * 0x3c4) = 0x4e;
    if (sv_PlayerBarForm != -1) {
        if (sv_PanelForm != -1)
            d2_SetViewEnabled(sv_PanelForm, 0);
        d2_SetForm(sv_PlayerBarForm, 1);
        for (p = &ai_Unk75f280[2]; (int)p < ((unsigned)&ai_Unk75f280 + 0x508); p += 10) {
            p[-2] = -1;
            p[0] = -1;
            p[1] = -1;
            p[2] = 0xffff;
            p[3] = -1;
            p[4] = -1;
            p[5] = 0;
        }
        d2_SetForm(sv_PlayerBarForm, 2);
        f = d2_WinOpenPicStyled(0x11, 4, 0x51d, d2_LastWindow, -2);
        *(int *)(d2_Objs + 0x4c + f * 0x2ec) = 1;
        *(int *)(d2_Objs + 0x1c0 + f * 0x2ec) = 3;
        for (c = ai_Chars[0] + 0x188; (int)c < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x186); c += 0x21c)
            if (*c != 0 && c[-0x24] != 0)
                *c = 0;
    }
}
