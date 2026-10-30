/* 0x531780, the window procedure of "Die Gilde": WM_DESTROY / WM_CLOSE post the quit message, WM_PAINT / WM_MOUSEWHEEL validate the window, WM_SYSCOLORCHANGE revalidates and repaints the screen
   (d2_BlitPart copies the back surface 0x6ed92c onto the screen surface 0x6ed924) when both surfaces are set, WM_ACTIVATEAPP restores the screen and reacquires the sound on activation (after
   the flip waits) and releases everything on deactivation (window to the bottom of the z order), 0x218 with wparam 0 answers the magic 0x424d5144 ("DQMB"), 0x7e7 reacquires the sound;
   everything else goes to DefWindowProc. The surfaces are read from their globals at every use, like the original @deep body complete and instruction-identical to the original (dcp diffclass: hunks=0, 'identical instructions', 185 = 185 ins; the final fix was the '!= 1' nesting of the deactivation branch and tb_Paused = 0 before sfx_ReacquireAll). The grade still reads code=draft: dcp grade B3 'code 19' / size 590 vs 627 and B1 '2 of 640 bytes differ at +0xb4' are the original's 37-byte tail inside the function extent (0x5319ce.. fill, the 5-entry switch jump table 0x5319d0.. and the 0x16-byte index table 0x5319e4..) which our function symbol does not cover (the same walker-extent artifact as ai_PokerGame's note); nothing left to change in the C */

#define SCREEN ((struct D2SURF *)d2_ScreenSurf)
#define BACK ((struct D2SURF *)d2_BackSurf)

LRESULT CALLBACK main_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case 0x2:
    case 0x10:
        PostQuitMessage(0);
        return 0;
    case 0xf:
        ValidateRect(hwnd, 0);
        return 0;
    case 0x14:
        ValidateRect(hwnd, 0);
        if (SCREEN != 0 && BACK != 0)
            d2_BlitPart(0, 0, SCREEN->w, SCREEN->h, BACK, 0, 0, SCREEN);
        return 1;
    case 0x1c:
        if (wparam != 0) {
            if (main_Inactive != 0) {
                if (main_ScreenOpen != 0) {
                    while (dd_Restore() == 0)
                        ;
                    while (d3_Hardware != 0 && d3d_Restore() == 0)
                        ;
                    d2_FlushDirty();
                    if (d2_LockSurface(SCREEN) != 0) {
                        d2_BlitPart(0, 0, SCREEN->w, SCREEN->h, BACK, 0, 0, SCREEN);
                        d2_DrawObjects();
                        d2_UnlockSurface(SCREEN);
                        dd_Flip();
                        d2_BlitPart(0, 0, SCREEN->w, SCREEN->h, BACK, 0, 0, SCREEN);
                    }
                    tb_Paused = 0;
                    sfx_ReacquireAll(hwnd, 0x7e7);
                    di_AcquireMouse(1);
                    di_AcquireKeyboard(1);
                }
                main_Inactive = 0;
            }
        }
        else {
            if (main_Inactive != 1) {
                main_Inactive = 1;
                if (main_ScreenOpen != 0) {
                    di_AcquireMouse(0);
                    di_AcquireKeyboard(0);
                    sfx_ReleaseAll();
                    tb_Paused = 1;
                }
                SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, 3);
            }
        }
        return 0;
    case 0x20a:
        ValidateRect(hwnd, 0);
        return 1;
    case 0x218:
        if (wparam == 0)
            return 0x424d5144;
        break;
    case 0x7e7:
        sfx_Reacquire(wparam, hwnd, 0x7e7);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}
