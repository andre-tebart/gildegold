/* 0x43bc20, starts the engine: copies the configuration, sets the display mode (and Direct3D), allocates
   the sort buffers and the shadow object, sets the viewport and fills the noise table.
   @deep Q2: d3io_NoopCallback has no evidence (d3_io_a.c 0x44ee40: the empty no-op triangle filler assigned to d3_TriFuncs[0..5], called with different signatures, recorded @deep there); the list heads / sentinels d3_StockListHead = &d3_StockListEnd, d3_AnimListHead = d3_AnimListEnd, d3_Unkfc4c40 = d3_Unkfc2e00, d3_Unkfe75a4 = d3_Unkfc2580 and the flags d3_Unkfc26e4 / d3_Unkfc4c3c / d3_Unkfc3000 / d3_Unkfe75a0 / d3_Unkfe7400 / d3_Unkfc5064 / d3_Unkfc5065 / d3_Unkfe7600 / d3_Unkfe75bc are only stored here (write-only, header comments state the list role but not a reader; the d3_io_t.h names d3_StockHead / d3_AnimHead / d3_AnimList are different struct views at the same addresses and cannot replace the D3OPS / void* view); d3s_Object = d3s_Object would need its extern moved to d3s.h, which flipped d3_CloneObject and d3_MirDrawObject when tried (notes/units/d3_engine.md); d3_Unk1463570 / 1463580 / 1464fbd are dd_RBits / dd_GBits / dd_BBits (dd_vesa.c) but d3_d3d.h declares them char where dd_vesa.c defines unsigned char and d3_d3d.c 0x436xxx reads them (the byte-matched use needs the unsigned char view). Renamed this session: d3_Camera / d3_Scenes / d3_AnimPath / d3_ShadeInit / d3fl_Init / par_Init. Q4: the D3_ZERO_AT-style macro raw dereference. */

char d3_Init(D3INITCFG *cfg, HWND wnd, int unk)
{
    int i, k, n;
    unsigned char x, y, c;

    d3_D3DActive = 0;
    d3_Unk633438 = 0;
    D3E_COPY_AT(offset d3_Cfg, cfg, sizeof(D3INITCFG));
    if (d3_Cfg.viewW + d3_Cfg.viewX > (float)d3_Cfg.w)
        d3_Cfg.viewW = (float)d3_Cfg.w - d3_Cfg.viewX;
    if (d3_Cfg.viewH + d3_Cfg.viewY > (float)d3_Cfg.h)
        d3_Cfg.viewH = (float)d3_Cfg.h - d3_Cfg.viewY;
    if ((d3_Cfg.mode == 0 || d3_Cfg.mode == 2 || d3_Cfg.mode == 4) && d3_Cfg.d3d)
        d3_Cfg.d3d = 0;
    if (d3_Cfg.mode) {
        if (!dd_Init(d3_Cfg.mode, d3_Cfg.bpp, wnd, unk))
            return 0;
        if (d3_Cfg.d3d)
            d3_D3DActive = d3d_Init(d3_Cfg.bpp, (int)wnd, unk);
        else
            d3_D3DActive = 0;
    }
    if (!dd_SetMode(d3_Cfg.w, d3_Cfg.h, d3_Cfg.bpp, d3_Cfg.mode, wnd, unk, d3_Cfg.triple))
        return 0;
    for (i = 0; i < 128; i++)
        d3_ClipPool[i].flags &= 0x7f;
    d3_ClipNone = 6;
    d3_Unkfc5064 = 0;
    d3_Unkfc5065 = 0;
    dd_SetClipRect((int)d3_Cfg.viewX, (int)d3_Cfg.viewY, (int)(d3_Cfg.viewW + d3_Cfg.viewX - 1.0f),
                   (int)(d3_Cfg.viewH + d3_Cfg.viewY - 1.0f));
    d3_TriFuncs[0] = d3io_NoopCallback;
    d3_TriFuncs[1] = d3io_NoopCallback;
    d3_TriFuncs[3] = d3io_NoopCallback;
    d3_TriFuncs[2] = d3io_NoopCallback;
    d3_TriFuncs[4] = d3io_NoopCallback;
    d3_TriFuncs[5] = d3io_NoopCallback;
    d3_SortBuf = mem_Alloc(d3_Cfg.maxpolys * sizeof(D3SORTENT), "d3:PolyList1");
    d3_SortTmp = mem_Alloc(d3_Cfg.maxpolys * sizeof(D3SORTENT), "d3:PolyList2");
    d3_SortPtr = d3_SortBuf;
    d3_SortCount = 0;
    d3_ObjEnd.flags |= 8;
    d3_TopHead.flags |= 8;
    d3_InitObject(&d3_TopHead);
    d3_InitObject(&d3_ObjEnd);
    d3_TopFirst = &d3_ObjEnd;
    d3_TopLast = &d3_TopHead;
    d3_Camera = 0;
    d3_StockListHead = &d3_StockListEnd;
    d3_Unkfc4c3c = 0;
    d3_Unkfc3000 = 0;
    d3_Unkfc4c40 = d3_Unkfc2e00;
    d3_Unkfe75a0 = 0;
    d3_Unkfc26e4 = 0;
    d3_AnimListHead = d3_AnimListEnd;
    d3_Unkfe75a4 = d3_Unkfc2580;
    d3_AnimPath = 0;
    d3_ShadeInit(0x200, 1);
    d3_FrameCount = 0;
    d3_Unkfe7400 = 0;
    tex_Init(d3_Cfg.texcount, 0, d3_Cfg.texParam, 0x40);
    if (d3_D3DActive) {
        d3_D3DActive = d3d_CreateDevice(8, d3_Cfg.texbpp, d3_Unk1463570 + d3_Unk1463580 + d3_Unk1464fbd, d3_Cfg.zbpp);
        if (d3_D3DActive) {
            d3d_SetConfig(d3_DefaultD3DConfig);
            d3d_SetWBuffer(1);
            d3_FogColor = 0;
            d3d_TexShift = d3_Cfg.texShift;
        }
    }
    d3fl_Init(0x400, 0x40 >> d3_Cfg.flShift, d3_Cfg.flArg, d3_Cfg.flFlag);
    d3_InitShadows(d3_Cfg.shadowSize, 0x80, 0x40, 0x20, 0, 0x40, 0x20);
    d3sky_InitColors();
    d3s_Object = d3_SpawnObject(4, "shadow_fake");
    D3_X(d3s_Object)->unk214 = 3;
    d3s_Object->objectFlags |= 0x20;
    d3_AddObjectDrawData(d3s_Object);
    D3_MESH(d3s_Object)->nsurf = 1;
    d3s_Object->info = &D3_MESH(d3s_Object)->surf[0].info;
    d3s_Object->info->mesh = 0;
    d3s_Object->info->nkeys = (d3s_MaxPolys >> 4) * 25;
    d3s_Object->info->ntris = d3s_MaxPolys * 2;
    d3_AllocObjectPolys(d3s_Object->info);
    d3s_Object->info->nkeys = 0;
    d3s_Object->info->ntris = 0;
    d3_SetHardwareMode((*(struct D3BITS8 *)&d3_D3DActive).on);
    d3_EngineReady = 1;
    d3_SetViewport(d3_Cfg.viewW, d3_Cfg.viewH, d3_Cfg.viewX, d3_Cfg.viewY, d3_Cfg.zn, d3_Cfg.zf, d3_Cfg.scale,
                   d3_Cfg.w, d3_Cfg.h);
    d3_CurrentScene = 0;
    d3_Unkfe7600 = (float)atan2(d3_HalfW, d3_ScaleX);
    d3_Unkfe75bc = (float)atan2(d3_HalfH, d3_ScaleY);
    D3_ZERO_AT(offset d3_Scenes, sizeof(d3_Scenes));
    for (i = 0; i < 256; i++)
        for (k = 0; k < 256; k++)
            d3_Noise[i][k] = rand() * 256 / RAND_MAX;
    for (n = 15; n; n--) {
        for (i = 0; i < 256; i++) {
            for (k = 0; k < 256; k++) {
                x = rand() * 256 / RAND_MAX;
                y = rand() * 256 / RAND_MAX;
                c = d3_Noise[x][y];
                d3_Noise[x][y] = d3_Noise[i][k];
                d3_Noise[i][k] = c;
            }
        }
    }
    par_Init();
    return 1;
}
