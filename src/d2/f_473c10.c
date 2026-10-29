/* 0x473c10, d2_Open (error strings): allocates the lists, reads the picture table of the gfx file dir\gfx\file,
   loads the locked pictures, makes the surfaces and finds the standard pictures; 0 on failure */

int d2_Open(int hwnd, int w, int h, char *dir, char *file)
{
    int desc[16];
    char path[256];
    char sd0[0x7c];
    char sd1[0x7c];
    int i;
    char *p;

    d2_MainWindow = 0;
    d2_DrawOrder = mem_Alloc(0xc00, "d2:zorder");
    d2_Objs = mem_Alloc(0x8c400, "d2:obj");
    d2_Pics = mem_Alloc(0x2a000, "d2:fileobj");
    d2_Dirty[0] = mem_Alloc(0x3000, "d2:d2_RedrawInfo0");
    d2_Dirty[1] = mem_Alloc(0x3000, "d2:d2_RedrawInfo1");
    d2_RedrawInfo2 = mem_Alloc(0x3000, "d2:d2_RedrawInfo2");
    d2_Shapes = mem_Alloc(0x3c00, "d2:RedrawInfoVirtual");
    memset(d2_Wins, 0, 0x60 * sizeof(D2WIN));
    d2_Objs[0].id = 0;
    memset(d2_Pics, 0, 4);   /* the original clears the first 4 bytes with one dword store */
    d2_DrawOrder[0] = 0;
    memset(&d2_DirtyRectLeft, 0, 16);   /* d2_DirtyRectLeft..6c, four ints (one memset, not four stores) */
    memset(d2_Fades, 0, sizeof(d2_Fades));
    for (i = 0; i < 0x30; i++)
        d2_Views[i].id = i;
    for (i = 0; i < 0x60; i++)
        d2_Wins[i].id = i;
    for (i = 0; i < 0x300; i++)
        d2_Objs[i].id = i;
    d2_PixelFormat = dd_PixelFormat;
    if (d2_File)
        return 0;
    sprintf(path, "%sgfx\\%s", dir, file);
    strcpy(d2_GfxPath, path);
    strcpy(d2_GfxDir, path);
    p = strrchr(d2_GfxDir, '\\');
    if (p)
        p[1] = 0;
    d2_File = fopen(path, "rb");
    if (!d2_File)
        return 0;
    fread(&d2_NPics, 4, 1, d2_File);
    if (d2_NPics > 0x800) {
        fclose(d2_File);
        err_Message("d2_Open:Too many Objects in GFX-File...");
        return 0;
    }
    for (i = 0; i < d2_NPics; i++)
        fread(&d2_Pics[i], 0x54, 1, d2_File);
    fclose(d2_File);
    d2_SetColourFormat(0, 1);
    for (i = 0; i < d2_NPics; i++)
        if ((d2_Pics[i].flags & 1) && d2_Pics[i].size)
            d2_LoadPic(i, 0);
    d2_ScreenScale = (float)w * 0.00125f;
    if (w == 800)
        d2_PicBase = 0;
    else if (w == 0x400)
        d2_PicBase = 1;
    else if (w == 0x480)
        d2_PicBase = 2;
    d2_MainWindow = hwnd;
    d2_ScreenH = h;
    d2_ScreenW = w;
    d2_SetClipRect(0, 0, w, h);
    d2_HalfScreenW = dd_Width / 2;
    d2_HalfScreenH = dd_Height / 2;
    memset(desc, 0, sizeof desc);
    desc[1] = dd_Width;
    desc[2] = dd_Height;
    d2_BackSurf = d2_MakeSurface(desc, 0xb);
    if (!d2_BackSurf)
        return 0;
    memset(desc, 0, sizeof desc);
    desc[1] = 0x100;
    desc[2] = 0x100;
    d2_BackgroundTile = d2_MakeSurface(desc, 3);
    if (!d2_BackgroundTile)
        return 0;
    memset(desc, 0, sizeof desc);
    desc[1] = dd_Width;
    desc[2] = dd_Height;
    desc[8] = (int)dd_BackBuffer;
    d2_ScreenSurf = d2_MakeSurface(desc, 4);
    if (!d2_ScreenSurf)
        return 0;
    memset(desc, 0, sizeof desc);
    desc[1] = dd_Width;
    desc[2] = dd_Height;
    desc[8] = (int)dd_FrontBuffer;
    d2_FrontSurf = d2_MakeSurface(desc, 6);
    if (!d2_FrontSurf)
        return 0;
    d2_GetSurfaceDesc(d2_ScreenSurf, sd0);
    d2_GetSurfaceDesc(d2_BackSurf, sd1);
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_SetFont(d2_FindPic("_FONT"));
    d2_CaptionTex = (D2TEX *)d3if_LoadSprite("BLACK", 0);
    if (!d2_CaptionTex)
        err_Message("d2_Open: Could not init transparency...");
    d2_ScrollMarkerPic = d2_FindPic("_WIN_ICONS");
    d2_FramePic = d2_FindPic("_SLIDER_INPUT");
    d2_DefaultFont = d2_FindPic("_FONT");
    d2_ButtonPulldownPic = d2_FindPic("_BUTTON_PULLDOWN");
    d2_WinBorderPic = d2_FindPic("_WIN_BORDER");
    d2_WinBorderPergaPic = d2_FindPic("_WIN_BORDER_PERGA");
    d2_MiscPic = d2_FindPic("_MISC");
    d2_PointerPicIndex = d2_FindPic("_MOUSE_CURSOR");
    i = d2_FindPic("_EXTRA_SYMBOLS");
    d2_ExtraSymbolsPic = i;
    d2_TextCursorPic = d2_FindPic("_TEXT_CURSOR");
    if (d2_TextCursorPic == -1)
        d2_TextCursorPic = i + 8;
    return 1;
}
