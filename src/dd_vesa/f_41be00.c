/* 0x41be00, releases everything dd_SetMode created */

void dd_Exit(void)
{
    dd_Surface = NULL;
    dd_Cleared = 0;
    dd_ZCleared = 0;
    if (dd_Mode == DD_FULLMEM)
        dd_Mode = DD_FULLSCREEN;
    else if (dd_Mode == DD_WINDOWMEM)
        dd_Mode = DD_WINDOW;
    switch (dd_Mode) {
    case DD_GDI:
        DeleteObject(dd_Bitmap);
        break;
    case DD_WINDOW:
        if (lpClipper) {
            dd_Ref = lpClipper->lpVtbl->Release(lpClipper);
            lpClipper = NULL;
        }
        if (dd_FrontBuffer) {
            dd_Ref = dd_FrontBuffer->lpVtbl->Release(dd_FrontBuffer);
            dd_FrontBuffer = NULL;
        }
        if (dd_BackBuffer) {
            dd_Ref = dd_BackBuffer->lpVtbl->Release(dd_BackBuffer);
            dd_BackBuffer = NULL;
        }
        if (dd_DD) {
            dd_Ref = dd_DD->lpVtbl->Release(dd_DD);
            dd_DD = NULL;
        }
        if (dd_DDBase) {
            dd_Ref = dd_DDBase->lpVtbl->Release(dd_DDBase);
            dd_DDBase = NULL;
        }
        if (dd_MemBuff) {
            mem_Free(dd_MemBuff);
            dd_MemBuff = NULL;
        }
        break;
    case DD_FULLSCREEN:
        if (dd_FrontBuffer) {
            dd_Ref = dd_FrontBuffer->lpVtbl->Release(dd_FrontBuffer);
            dd_FrontBuffer = NULL;
        }
        if (dd_DD) {
#line 473
            D3DCHECK(dd_DD->lpVtbl->SetCooperativeLevel(dd_DD, dd_Window, DDSCL_NORMAL));
            dd_Ref = dd_DD->lpVtbl->Release(dd_DD);
            dd_DD = NULL;
        }
        if (dd_DDBase) {
            dd_Ref = dd_DDBase->lpVtbl->Release(dd_DDBase);
            dd_DDBase = NULL;
        }
        if (dd_MemBuff) {
            mem_Free(dd_MemBuff);
            dd_MemBuff = NULL;
        }
        break;
    }
}
