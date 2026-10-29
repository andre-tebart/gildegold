/* 0x52d480, with the key flag 8 set and the camera rotated away from the stored views (0x5ebba4 zero vector, tolerance 0.2): draws the arrows (picture 0x52c, frames 0 .. 7) at the screen border that show
   the way back to the object's view: the pairs 0xfc2dec +0xe4 / +0xfc (position side) and +0xb4 / +0xcc (direction side) decide which of two arrows per side is shown; ROUGH */

struct SBSAMPLE * sb_PlaySample();                     /* 0x426fa0 */
int ai_Unk576340();                     /* 0x576340 */
int ai_Unk575860();                     /* 0x575860 */
int ai_Unk591170();                     /* 0x591170 */
int anm_Update();                     /* 0x4bb860 */
int ai_Unk59e0a0();                     /* 0x59e0a0 */
void sv_ReleaseNamePlates(void);                /* 0x52e190 */
void gm_GetGebaeudeHauptraum(unsigned char *b);                     /* 0x501ce0 */

void sv_Unk52d480(void)
{
    int pic, n3, n5, o;
    char a, b;
    char *o1;

    a = 0;
    b = 0;
    n3 = 0;
    o = 0;
    if ((SV_B((unsigned)&sv_FrameMask) & 8) == 0)
        return;
    o1 = SV_C((unsigned)&ai_PlayerBuilding);
    if ((o1 != 0 || (SV_C((unsigned)&ai_FallbackBuilding) != 0 && (o1 = SV_C((unsigned)&ai_FallbackBuilding)) != 0)) && (SV_I((unsigned)&sv_PanelShown) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
        && (SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, o1, SV_I((unsigned)&sv_PanelShown), 0), (SV_I((unsigned)&full_data_6f2320) & 0x1000) == 0))
        return;
    if ((SV_I((unsigned)&ai_BuildingPerms) & 0x1000) == 0)
        return;
    pic = d2_GetPic(0x52c);
    if (SV_I((unsigned)&ai_Unk6f22cc) == -1) {
        n3 = 1;
        if (d3_VecEqual(ai_Unkfc2dec + 0xfc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            n3 = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22cc) == 1) {
        b = 1;
        if (d3_VecEqual(ai_Unkfc2dec + 0xe4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            b = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22cc) == 0) {
        b = d3_VecEqual(ai_Unkfc2dec + 0xe4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0;
        if (d3_VecEqual(ai_Unkfc2dec + 0xfc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            n3 = 1;
    }
    if (SV_I((unsigned)&ai_Unk6f22d0) == -1) {
        a = 1;
        if (d3_VecEqual(ai_Unkfc2dec + 0xb4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            a = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22d0) == 1) {
        o = SV_I((unsigned)&ai_Unk6f22d0);
        if (d3_VecEqual(ai_Unkfc2dec + 0xcc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            o = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22d0) == 0) {
        a = d3_VecEqual(ai_Unkfc2dec + 0xb4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0;
        if (d3_VecEqual(ai_Unkfc2dec + 0xcc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            o = 1;
    }
    d2_SetClipRect(0, 0, *(int *)((char *)d2_ScreenSurf + 4), *(int *)((char *)d2_ScreenSurf + 8));
    if (a == 2)
        d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, SV_S((unsigned)&d2_ScreenH) - 0x20, d2_ScreenSurf, pic, 0);
    else if (a == 1)
        d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, SV_S((unsigned)&d2_ScreenH) - 0x20, d2_ScreenSurf, pic, 1);
    if (b == 2)
        d2_DrawShape(4, SV_I((unsigned)&d2_HalfScreenH), d2_ScreenSurf, pic, 2);
    else if (b == 1)
        d2_DrawShape(4, SV_I((unsigned)&d2_HalfScreenH), d2_ScreenSurf, pic, 3);
    if (n3 == 2)
        n5 = 4;
    else if (n3 == 1)
        n5 = 5;
    else
        n5 = -1;
    if (n5 != -1)
        d2_DrawShape(SV_S((unsigned)&d2_ScreenW) - 0x96, SV_I((unsigned)&d2_HalfScreenH), d2_ScreenSurf, pic, n5);
    if (o != 2) {
        if (o == 1)
            d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, 0x38, d2_ScreenSurf, pic, 7);
        return;
    }
    d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, 0x38, d2_ScreenSurf, pic, 6);
}
