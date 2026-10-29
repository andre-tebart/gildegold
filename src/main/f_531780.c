/* 0x531780, the window procedure of "Die Gilde": WM_DESTROY / WM_CLOSE post the quit message, WM_PAINT / WM_MOUSEWHEEL validate the window, WM_SYSCOLORCHANGE revalidates and repaints the screen twice
   (0x479480 blits 0x6ed92c into 0x6ed924) when both surfaces are set, WM_ACTIVATEAPP restores the screen and reacquires the sound on activation (after the flip waits) and releases everything on
   deactivation (window to the bottom of the z order), 0x218 with wparam 0 answers the magic 0x424d5144 ("DQMB"), 0x7e7 reacquires the sound; everything else goes to DefWindowProc. ROUGH: the switch
   dispatch order (0x1c tested before the table) may differ */

LRESULT CALLBACK main_Unk531780(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    int *p;
    int q;

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
        p = (int *)U_I((unsigned)&d2_ScreenSurf);
        q = U_I((unsigned)&d2_BackSurf);
        if (p != 0 && q != 0)
            d2_BlitPart(0, 0, p[1], p[2], q, 0, 0, p);
        return 1;
    case 0x1c:
        if (wparam != 0) {
            if (U_I((unsigned)&full_data_6f2584) != 0) {
                if (*(char *)(unsigned)&main_ScreenOpen != 0) {
                    while (dd_Restore() == 0)
                        ;
                    while (*(char *)(unsigned)&d3_Hardware != 0 && d3d_Restore() == 0)
                        ;
                    d2_FlushDirty();
                    if (d2_LockSurface(U_I((unsigned)&d2_ScreenSurf)) != 0) {
                        p = (int *)U_I((unsigned)&d2_ScreenSurf);
                        q = U_I((unsigned)&d2_BackSurf);
                        d2_BlitPart(0, 0, p[1], p[2], q, 0, 0, p);
                        d2_DrawObjects();
                        d2_UnlockSurface(U_I((unsigned)&d2_ScreenSurf));
                        dd_Flip();
                        d2_BlitPart(0, 0, p[1], p[2], q, 0, 0, p);
                    }
                    sfx_ReacquireAll(hwnd, 0x7e7);
                    U_I((unsigned)&tb_Paused) = 0;
                    di_AcquireMouse(1);
                    di_AcquireKeyboard(1);
                }
                U_I((unsigned)&full_data_6f2584) = 0;
            }
        }
        else {
            if (U_I((unsigned)&full_data_6f2584) == 1)
                return 0;
            U_I((unsigned)&full_data_6f2584) = 1;
            if (*(char *)(unsigned)&main_ScreenOpen != 0) {
                di_AcquireMouse(0);
                di_AcquireKeyboard(0);
                sfx_ReleaseAll();
                U_I((unsigned)&tb_Paused) = 1;
            }
            SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, 3);
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
