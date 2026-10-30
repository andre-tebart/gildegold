/* 0x531c50, opens the 3D / 2D screen for the display mode m (3 and 4 = full screen windows, topmost): picks a supported resolution (0 = 800x600, 1 = 1024x768, 2 = 1152x864: the chosen one is
   kept when its mode exists, else the first of the others that does), sizes and centres the window on the work area, fills the video record (the DXT1 texture format 0x31545844, the screen size,
   the Gfx/TextureDivider key, the view scale 512 / (640 / width)) for d3_Init, starts the input (full screen mode 3: exclusive keyboard), sets the directories ("animations/", "objects/",
   "groups/", "scripts/", "forms\\") and opens the 2D layer with "gilde_add_on_<language>.gfx"; returns 1 on success. Reasoned: when AdjustWindowRectEx fails the original takes the window
   exstyle for both the width and the height @deep body reconstructed and readable (video record = VIDCFG / d3_engine.h's D3INITCFG with the triple field a dword, resolution fallback chain 800 -> 1024 -> 1152, window sizing with the original's failure path w = h = GetWindowLong(EXSTYLE), view scale 512 / (640 / (double)(unsigned)w), directories, d2_Open); 337 = 337 instructions. Left (asmdiff, 226 ins in 10 hunks, allocation / block layout): (1) the original keeps the exstyle in a stack slot [esp+0x10] (reloaded for the AdjustWindowRectEx argument and for both w and h on failure) and caches GetWindowLongA's import in ebp (call ebp twice), work.right-left / bottom-top in esi / edi, all four loaded right after SystemParametersInfoA; ours keeps ex in esi, loads the work fields in another order and calls through the thunk twice; (2) x / y = work - w / h computed late (first form) gives the original's shape but moves the video record 8 bytes (the 64-bit fild temp of the scale shares the ex slot in the original), computed early (current form) keeps the frame but not the schedule; (3) the resolution chain: the original lays out 'res==0 && 800 ok -> done; 1024 ? al=1 : 1152 ? al=2' inline right after the first test and a shared 'xor al,al' tail for the 800 fallback, ours (gotos pick1024 / try800) emits the blocks in another order (hunks at 0x531d4e / 0x531d70); (4) after d3_Init the three-register rotation of the input / path set-up (regs eax->edx ecx->eax edx->ecx, 20 ins). Data fixes made: ai_DataPath / d3_AnimDir / d2_FormsDir typed as char[], ai_ScreenModes as {w,h}[], full_data_6f242c -> main_TextureDivider, full_data_624690 -> main_GamePath, the 'groups/' literal views removed */

int sv_PumpMessages();

int main_OpenScreen(int m)
{
    char path[260];
    RECT work;
    VIDCFG cfg;
    RECT rc;
    int w, h, x, y, ex;

    cfg.w = 0;
    cfg.h = 0;
    cfg.bpp = 0x10;
    cfg.mode = 1;
    cfg.d3d = 1;
    cfg.triple = 0;
    cfg.maxpolys = 0x10000;
    cfg.texcount = 0x1000;
    cfg.maxhicol = 600;
    cfg.texParam = 8;
    cfg.zbpp = 0x10;
    cfg.texbpp = 0;
    cfg.flFlag = 0;
    cfg.flShift = 0;
    cfg.flArg = 0;
    cfg.texShift = 0x20;
    cfg.shadowSize = 0x284;
    cfg.viewW = 456.0f;
    cfg.viewH = 78.0f;
    cfg.viewX = 50.0f;
    cfg.viewY = 4.0f;
    cfg.zn = 12000.0f;
    cfg.zf = 512.0f;
    cfg.scale = 0.0f;
    ai_LoadOptions();
    if (dd_CheckModes(m, 0x10, main_Window, (int)main_Instance) == 0)
        return 0;
    if (dd_Has800x600 == 0 && dd_Has1024x768 == 0 && dd_Has1152x864 == 0)
        return 0;
    if (ai_GfxResolution == 0) {
        if (dd_Has800x600 != 0)
            goto sized;
pick1024:
        if (dd_Has1024x768 == 0) {
            if (dd_Has1152x864 != 0)
                ai_GfxResolution = 2;
        } else
            ai_GfxResolution = 1;
    } else if (ai_GfxResolution == 1) {
        if (dd_Has1024x768 != 0)
            goto sized;
        goto try800;
    } else if (ai_GfxResolution == 2) {
        if (dd_Has1152x864 != 0)
            goto sized;
try800:
        if (dd_Has800x600 == 0)
            goto pick1024;
        ai_GfxResolution = 0;
    } else
        goto sized;
    ai_ScreenModeW = ai_ScreenModes[ai_GfxResolution].w;
    ai_ScreenModeH = ai_ScreenModes[ai_GfxResolution].h;
sized:
    sv_ViewRectLeft = 0;
    sv_ViewRectTop = 0;
    sv_ViewRectRight = ai_ScreenModeW - 0x6a;
    sv_ViewRectBottom = ai_ScreenModeH;
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
    x = work.right - work.left;
    y = work.bottom - work.top;
    rc.left = 0;
    rc.top = 0;
    rc.right = ai_ScreenModeW;
    rc.bottom = ai_ScreenModeH;
    ex = GetWindowLongA(main_Window, GWL_EXSTYLE);
    w = h = ex;
    if (AdjustWindowRectEx(&rc, GetWindowLongA(main_Window, GWL_STYLE), 0, ex)) {
        w = rc.right - rc.left;
        h = rc.bottom - rc.top;
    }
    x -= w;
    y -= h;
    if (m == 3 || m == 4)
        SetWindowPos(main_Window, HWND_TOPMOST, abs(abs(y) / 2), abs(abs(x) / 2), w, h, 0);
    else
        SetWindowPos(main_Window, 0, abs(y) / 2, abs(x) / 2, w, h, 0);
    sv_PumpMessages();
    cfg.w = ai_ScreenModeW;
    cfg.h = ai_ScreenModeH;
    cfg.viewW = (float)ai_ScreenModeW;
    cfg.viewH = (float)ai_ScreenModeH;
    cfg.zbpp = 0x20;
    cfg.shadowSize = 0x20;
    d3_DefaultD3DConfig[1] = (d3_DefaultD3DConfig[1] & 0xf9) | 1;
    cfg.texShift = main_TextureDivider;
    cfg.bpp = 0x10;
    cfg.mode = m;
    cfg.d3d = 1;
    cfg.triple = 0;
    cfg.viewX = 0.0f;
    cfg.viewY = 0.0f;
    cfg.flFlag = 1;
    cfg.flShift = 0;
    cfg.flArg = 0;
    cfg.texParam = 4;
    cfg.texbpp = 0x31545844;
    cfg.maxpolys = 64000;
    cfg.texcount = 0x1000;
    cfg.maxhicol = 0;
    cfg.zn = 4.0f;
    cfg.zf = 12000.0f;
    d3io_Lod = 2;
    cfg.scale = (float)(512.0 / (640.0 / (double)cfg.w));
    if (d3_Init(&cfg, main_Window, main_Instance) != 0) {
        d3if_CreateCameras();
        while (ShowCursor(0) > 0)
            ;
        if (cfg.mode == 3) {
            di_Init(main_Window, main_Instance, 1);
            kb_SetBase(0x50);
        } else
            di_Init(main_Window, main_Instance, 6);
        ai_ApplyOptions();
        tex_SetPalette("textures/");
        strcpy(d3_AnimDir, "animations/");
        d3io_SetStockDir("objects/");
        strcpy(ai_DataPath, "groups/");
        strcpy(evt_Path, "scripts/");
        sprintf(d2_FormsDir, "forms\\");
        ai_ApplyGfxOptions();
        sprintf(path, "gilde_add_on_%s.gfx", "german");  /* the pooled literal 0x5f8ad0: 0x53ccf0 reads General/Language into the same string (its default) */
        if (d2_Open((int)main_Window, ai_ScreenModeW, ai_ScreenModeH, main_GamePath, path) != 0) {
            di_MaxY = d2_ScreenH;
            di_MaxX = d2_ScreenW - 8;
            d2_MainWindow = (int)main_Window;
            d2_CreateItemListSurfaces();
            sv_PumpMessages();
            return 1;
        }
    }
    return 0;
}
