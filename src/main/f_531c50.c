/* 0x531c50, opens the 3D / 2D screen for the display mode m (3 and 4 = full screen windows, topmost): the video setup record (24 ints: 0x43bc20 reads it: size, mode, flags, the DXT1 format 0x31545844 ..) is
   filled from the chosen resolution 0x62d7d0 / 0x62d7d4 (0x631225 / 0x631226 / 0x631227 = the available 640 / 800 / 1024 modes, 0x6f2f94 the chosen one), the window is sized and centred on the work area,
   the paths "textures/", "animations/", "objects/", "scripts/", "forms\\" and "gilde_add_on_<language>_gfx" are set and the interface is opened (0x473c10); returns 1 on success */

int sv_PumpMessages();
int sv_RunFrame(int cb, unsigned mask);

/* ---- layout 2026-10-23 (session 5): the window / engine start (ROUGH) ---- */

int main_Unk531c50(int m)
{
    int st[24];
    RECT rc, work;
    char path[260];
    DWORD ex, style;
    int w, h, ax, ay;

    st[0] = 0;
    st[1] = 0;
    st[2] = 0x10;
    st[3] = 1;
    st[4] = 1;
    st[5] = 0;
    st[6] = 0x10000;
    st[7] = 0x1000;
    st[8] = 600;
    st[9] = 8;
    st[10] = 0x10;
    st[11] = 0;
    st[12] = 0;
    st[13] = 0;
    st[14] = 0;
    st[15] = 0x20;
    st[16] = 0x284;
    *(float *)&st[17] = 456.0f;
    *(float *)&st[18] = 78.0f;
    st[19] = 0x42480000;
    st[20] = 0x40800000;
    st[21] = 0x463b8000;
    st[22] = 0x44000000;
    *(float *)&st[23] = 0.0f;
    ai_LoadOptions();
    if ((char)dd_CheckModes(m, 0x10, U_I((unsigned)&main_Window), U_I((unsigned)&main_Instance)) == 0)
        return 0;
    if (*(char *)(unsigned)&dd_Has800x600 == 0 && *(char *)(unsigned)&dd_Has1024x768 == 0 && *(char *)(unsigned)&dd_Has1152x864 == 0)
        return 0;
    if (*(unsigned char *)(unsigned)&ai_GfxResolution == 0) {
        if (*(char *)(unsigned)&dd_Has800x600 != 0)
            goto lab;
lc50:
        if (*(char *)(unsigned)&dd_Has1024x768 == 0) {
            if (*(char *)(unsigned)&dd_Has1152x864 != 0)
                *(unsigned char *)(unsigned)&ai_GfxResolution = 2;
        } else
            *(unsigned char *)(unsigned)&ai_GfxResolution = 1;
    } else {
        char c = *(char *)(unsigned)&dd_Has1024x768;

        if (((*(unsigned char *)(unsigned)&ai_GfxResolution != 1 && (c = *(char *)(unsigned)&dd_Has1152x864, *(unsigned char *)(unsigned)&ai_GfxResolution != 2))) || c != 0)
            goto lab;
        if (*(char *)(unsigned)&dd_Has800x600 == 0)
            goto lc50;
        *(unsigned char *)(unsigned)&ai_GfxResolution = 0;
    }
    U_I((unsigned)&ai_ScreenModeW) = U_I((unsigned)&ai_ScreenModes + *(unsigned char *)(unsigned)&ai_GfxResolution * 8);
    U_I((unsigned)&ai_ScreenModeH) = U_I(((unsigned)&ai_ScreenModes + 0x4) + *(unsigned char *)(unsigned)&ai_GfxResolution * 8);
lab:
    U_I((unsigned)&sv_ViewRectRight) = U_I((unsigned)&ai_ScreenModeW) - 0x6a;
    U_I((unsigned)&sv_ViewRectLeft) = 0;
    U_I((unsigned)&sv_ViewRectTop) = 0;
    U_I((unsigned)&sv_ViewRectBottom) = U_I((unsigned)&ai_ScreenModeH);
    SystemParametersInfoA(0x30, 0, &work, 0);
    rc.left = 0;
    rc.top = 0;
    rc.right = U_I((unsigned)&ai_ScreenModeW);
    rc.bottom = U_I((unsigned)&ai_ScreenModeH);
    ex = GetWindowLongA((HWND)U_I((unsigned)&main_Window), -0x14);
    style = GetWindowLongA((HWND)U_I((unsigned)&main_Window), -0x10);
    if (AdjustWindowRectEx(&rc, style, 0, ex) != 0) {
        ex = rc.right - rc.left;
        style = rc.bottom - rc.top;
    }
    w = (int)ex;
    h = (int)style;
    ax = work.right - work.left - w;
    ay = work.bottom - work.top - h;
    if (ax < 0)
        ax = -ax;
    if (ay < 0)
        ay = -ay;
    SetWindowPos((HWND)U_I((unsigned)&main_Window), (m == 3 || m == 4) ? (HWND)-1 : (HWND)0, ax / 2, ay / 2, w, h, 0);
    sv_PumpMessages();
    *(float *)&st[17] = (float)U_I((unsigned)&ai_ScreenModeW);
    *(float *)&st[18] = (float)U_I((unsigned)&ai_ScreenModeH);
    st[0] = U_I((unsigned)&ai_ScreenModeW);
    st[1] = U_I((unsigned)&ai_ScreenModeH);
    st[16] = 0x20;
    st[10] = 0x20;
    *(unsigned char *)((unsigned)&d3_DefaultD3DConfig + 0x1) = (*(unsigned char *)((unsigned)&d3_DefaultD3DConfig + 0x1) & 0xf9) | 1;
    st[15] = U_I((unsigned)&full_data_6f242c);
    st[2] = 0x10;
    st[3] = m;
    st[4] = 1;
    st[5] = 0;
    st[19] = 0;
    st[20] = 0;
    st[12] = 1;
    st[13] = 0;
    st[14] = 0;
    st[9] = 4;
    st[11] = 0x31545844;
    st[6] = 64000;
    st[7] = 0x1000;
    st[8] = 0;
    st[21] = 0x40800000;
    st[22] = 0x463b8000;
    U_I((unsigned)&d3io_Lod) = 2;
    *(float *)&st[23] = (float)(512.0 / (640.0 / (double)U_I((unsigned)&ai_ScreenModeW)));
    if ((char)d3_Init(st, U_I((unsigned)&main_Window), U_I((unsigned)&main_Instance)) != 0) {
        d3if_CreateCameras();
        while (ShowCursor(0) > 0)
            ;
        if (st[3] == 3) {
            di_Init(U_I((unsigned)&main_Window), U_I((unsigned)&main_Instance), 1);
            kb_SetBase(0x50);
        } else
            di_Init(U_I((unsigned)&main_Window), U_I((unsigned)&main_Instance), 6);
        ai_ApplyOptions();
        tex_SetPalette("textures/");
        strcpy((char *)(unsigned)&d3_AnimDir, "animations/");
        d3io_SetStockDir("objects/");
        U_I((unsigned)&ai_DataPath) = U_I((unsigned)&full_data_6249e8);
        U_I(((unsigned)&ai_DataPath + 0x4)) = U_I((unsigned)&full_data_6249ec);
        strcpy((char *)(unsigned)&evt_Path, "scripts/");
        crt_Sprintf((char *)(unsigned)&d2_FormsDir, "forms\\");
        ai_ApplyGfxOptions();
        crt_Sprintf(path, "gilde_add_on_%s.gfx", (char *)(unsigned)"german");
        if (d2_Open(U_I((unsigned)&main_Window), U_I((unsigned)&ai_ScreenModeW), U_I((unsigned)&ai_ScreenModeH), (char *)(unsigned)&full_data_624690, path) != 0) {
            U_I((unsigned)&di_MaxY) = *(short *)(unsigned)&d2_ScreenH;
            U_I((unsigned)&di_MaxX) = *(short *)(unsigned)&d2_ScreenW - 8;
            U_I((unsigned)&d2_MainWindow) = U_I((unsigned)&main_Window);
            d2_CreateItemListSurfaces();
            sv_PumpMessages();
            return 1;
        }
    }
    return 0;
}
