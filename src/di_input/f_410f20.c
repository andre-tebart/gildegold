/* 0x410f20, drain the buffered mouse events into dm_S (axes, wheel, buttons), clamp to the mouse limits, follow the cursor when di_Flags has bit 2, then di_UpdateButtons */

void di_PollMouse(void)
{
    int count = 1;
    DIDEVICEOBJECTDATA od;
    POINT pt;
    int v;

    dm_S.dx = 0;
    dm_S.dy = 0;
    dm_S.dz = 0;
    if (di_Mouse == 0)
        return;
    while (1) {
        dm_S.lclick = 0;
        dm_S.lup = 0;
        dm_S.lextra = 0;
        dm_S.rclick = 0;
        dm_S.rup = 0;
        dm_S.mclick = 0;
        dm_S.mup = 0;
        dm_S.wdown = 0;
        dm_S.wup = 0;
        v = di_Mouse->lpVtbl->GetDeviceData(di_Mouse, sizeof(od), &od, &count, 0);
        if (v < 0 || count == 0)
            break;
        switch (od.dwOfs) {
        case 0:
            dm_S.dx = (int)((float)(kb_Base * 0.00390625f) * (int)od.dwData);
            dm_S.x += dm_S.dx;
            break;
        case 4:
            dm_S.dy = (int)((float)(kb_Base * 0.00390625f) * (int)od.dwData);
            dm_S.y += dm_S.dy;
            break;
        case 8:
            v = (kb_Base * (int)od.dwData) >> 8;
            if (v > 0)
                dm_S.wup = 1;
            else if (v < 0)
                dm_S.wdown = 1;
            dm_S.z += v;
            dm_S.dz = v;
            di_UpdateButtons();
            break;
        case 0xc:

            if (od.dwData & 0x80) {
                dm_S.ldown = 1;
                dm_S.lup = 0;
                if (dm_L.rhold == 0)
                    dm_S.lclick = 1;
                di_SwapButtons();
                di_UpdateButtons();
            }
            if ((od.dwData & 0x80) == 0) {
                dm_S.lup = 1;
                dm_S.ldown = 0;
                di_SwapButtons();
                di_UpdateButtons();
            }
            break;
        case 0xd:

            if (od.dwData & 0x80) {
                dm_S.rdown = 1;
                dm_S.rup = 0;
                dm_S.rclick = 1;
                di_SwapButtons();
                di_UpdateButtons();
            }
            if ((od.dwData & 0x80) == 0) {
                dm_S.rup = 1;
                dm_S.rdown = 0;
                di_SwapButtons();
                di_UpdateButtons();
            }
            break;
        case 0xe:

            if (od.dwData & 0x80) {
                dm_S.mdown = 1;
                dm_S.mup = 0;
                dm_S.mclick = 1;
                di_SwapButtons();
                di_UpdateButtons();
            }
            if ((od.dwData & 0x80) == 0) {
                dm_S.mup = 1;
                dm_S.mdown = 0;
                di_SwapButtons();
                di_UpdateButtons();
            }
            break;
        }
    }
    if (dm_S.x < di_MinX)
        dm_S.x = di_MinX;
    if (dm_S.x > di_MaxX)
        dm_S.x = di_MaxX;
    if (dm_S.y < di_MinY)
        dm_S.y = di_MinY;
    if (dm_S.y > di_MaxY)
        dm_S.y = di_MaxY;
    if (di_Flags & 4) {
        GetCursorPos(&pt);
        ScreenToClient(di_Hwnd, &pt);
        if (di_Absolute == 0) {
            dm_S.x += pt.x - dm_L.x;
            dm_S.y += pt.y - dm_L.y;
            pt.x = dm_L.x;
            pt.y = dm_L.y;
            ClientToScreen(di_Hwnd, &pt);
            SetCursorPos(pt.x, pt.y);
            di_UpdateButtons();
            return;
        }
        dm_S.x = max(di_MinX, min(pt.x, di_MaxX));
        dm_S.y = max(di_MinY, min(pt.y, di_MaxY));
    }
    di_UpdateButtons();
}
