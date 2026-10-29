/* 0x524010, redraws the whole screen: the background view 0x6ed924 is filled (0x46d040), the panel 0x522f10 (1, 0) built into it, the view blitted (0x479480) to 0x6ed92c, a black clip rectangle ("BLACK") over the
   whole screen shown for one message round (0x5309a0 (0, 0x1c6)), the strip 0x623840 shown (0x4756b0) and the background copied back */

/* 0x46d040 */
void d2_BlitPart();                     /* 0x479480 */
struct D2FADE;
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */                     /* 0x4767d0 */
void d2_ReleaseFade(struct D2FADE *f); /* 0x476860, d2.h */                     /* 0x476860 */
void d2_PlaceWindow(int win, unsigned char flags);  /* 0x4756b0 */
int ahm_HasRecord();                     /* 0x4eaad0 */
struct D3SKY; struct D3SKYLAYER; struct D3VECTOR; int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */
char * ai_Unk4cda50(char *c);                     /* 0x4cda50 */

void sv_RedrawScreen(void)
{
    int r;

    d2_ClearSurface(d2_ScreenSurf);
    d2_DrawPic(0, 0, sv_PanelPic, d2_ScreenSurf);
    if (d2_LockSurface(d2_ScreenSurf) != 0) {
        sv_Unk522f10(1, 0);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_BlitPart(0, 0, *(int *)(ai_Unk6ed92c + 4), *(int *)(ai_Unk6ed92c + 8), d2_ScreenSurf, 0, 0, ai_Unk6ed92c);
    r = d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0, 1);
    sv_RunFrame(0, 0x1c6);
    d2_ReleaseFade(r);
    d2_PlaceWindow(ai_Unk623840, 1);
    d2_BlitPart(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, ai_Unk6ed92c, 0, 0, d2_ScreenSurf);
}
