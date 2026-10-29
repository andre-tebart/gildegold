/* 0x41d230, probes the large display modes: clears dd_Has800x600 / dd_Has1024x768 / dd_Has1152x864, opens
   DirectDraw through dd_Init when it is not open yet and releases it again afterwards, then enumerates the
   standard VGA modes (0x41d1c0 notes the flags); returns 1 when DirectDraw was available, 0 otherwise */

char dd_CheckModes(int mode, int bpp, HWND parent, int c)
{
    char created = 0;

    dd_Has800x600 = 0;
    dd_Has1024x768 = 0;
    dd_Has1152x864 = 0;
    if (!dd_DD) {
        dd_Init(mode, bpp, parent, c);
        created = 1;
    }
    if (dd_DD) {
#line 1209
        D3DCHECK(dd_DD->lpVtbl->EnumDisplayModes(dd_DD, DDEDM_STANDARDVGAMODES, NULL, (LPVOID)bpp, dd_EnumVideoModesCallback));
        if (dd_DD && created) {
            dd_Ref = dd_DD->lpVtbl->Release(dd_DD);
            dd_DD = NULL;
        }
        return 1;
    }
    return 0;
}
