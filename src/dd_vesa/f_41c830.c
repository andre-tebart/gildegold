/* 0x41c830, driver (102) and device (103) selection; the choice is stored as "dd_Guid"/"d3d_Guid" */

BOOL CALLBACK dd_DlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    LPDIRECTDRAW ddraw1;
    LPDIRECTDRAW4 ddraw4;
    char buf[0x200];
    char guidstr[0x100];
    int i, idx, sel = 0;
    HKEY cfg;
    GUID *guid;
    char *s;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case 102:
            i = SendMessage(dd_DrvCombo, CB_GETCURSEL, 0, 0);
            if (i == dd_LastDriver)
                return 1;
            dd_LastDriver = i;
            guid = (GUID *)SendMessage(dd_DrvCombo, CB_GETITEMDATA, i, 0);
            SendMessage(dd_DevCombo, CB_RESETCONTENT, 0, 0);
            d3d_NumDevices = 0;
            if (DirectDrawCreate(guid, &ddraw1, NULL) == DD_OK) {
#line 939
                D3DCHECK(ddraw1->lpVtbl->QueryInterface(ddraw1, &IID_IDirectDraw4, &ddraw4));
                if (!d3d_hr) {
                    LPDIRECT3D3 d3d3;
#line 944
                    D3DCHECK(ddraw4->lpVtbl->QueryInterface(ddraw4, &IID_IDirect3D3, &d3d3));
                    if (!d3d_hr) {
#line 947
                        D3DCHECK(d3d3->lpVtbl->EnumDevices(d3d3, d3d_EnumDeviceCallback, (void *)dd_BitPlanes));
                        dd_Ref = d3d3->lpVtbl->Release(d3d3);
                        d3d3 = NULL;
                    }
                    dd_Ref = ddraw4->lpVtbl->Release(ddraw4);
                    ddraw4 = NULL;
                }
                dd_Ref = ddraw1->lpVtbl->Release(ddraw1);
                ddraw1 = NULL;
            }
            for (i = 0; i < d3d_NumDevices; i++) {
                strcpy(buf, d3d_Devices[i].desc);
                strcat(buf, " (");
                strcat(buf, d3d_Devices[i].name);
                strcat(buf, ")");
                idx = SendMessage(dd_DevCombo, CB_ADDSTRING, 0, (LPARAM)buf);
                SendMessage(dd_DevCombo, CB_SETITEMDATA, idx, (LPARAM)&d3d_Devices[i].guid);
                if (!d3d_Devices[i].software)
                    sel = idx;
            }
            SendMessage(dd_DevCombo, CB_SETCURSEL, sel, 0);
            return 1;
        case IDOK:
            PostMessage(dlg, WM_CLOSE, 0, 0);
            return 1;
        }
        break;
    case WM_INITDIALOG:
        dd_DrvCombo = GetDlgItem(dlg, 102);
        dd_DevCombo = GetDlgItem(dlg, 103);
        SetWindowText(dlg, txt_DlgTitle);
        SetDlgItemText(dlg, 104, txt_DlgLabel104);
        SetDlgItemText(dlg, 105, txt_DlgLabel105);
        if (dd_DlgFlag) {
            SetWindowLong(dlg, GWL_STYLE, GetWindowLong(dlg, GWL_STYLE) & ~WS_CAPTION);
            EnableWindow(dd_DrvCombo, FALSE);
            while (ShowCursor(TRUE) < 1)
                ;
            SendMessage(dd_DevCombo, CB_RESETCONTENT, 0, 0);
            d3d_NumDevices = 0;
#line 889
            D3DCHECK(dd_D3D3->lpVtbl->EnumDevices(dd_D3D3, d3d_EnumDeviceCallback, (void *)dd_BitPlanes));
            for (i = 0; i < d3d_NumDevices; i++) {
                strcpy(buf, d3d_Devices[i].desc);
                strcat(buf, " (");
                strcat(buf, d3d_Devices[i].name);
                strcat(buf, ")");
                idx = SendMessage(dd_DevCombo, CB_ADDSTRING, 0, (LPARAM)buf);
                SendMessage(dd_DevCombo, CB_SETITEMDATA, idx, (LPARAM)&d3d_Devices[i].guid);
                if (!d3d_Devices[i].software)
                    sel = idx;
            }
            SendMessage(dd_DevCombo, CB_SETCURSEL, sel, 0);
            BringWindowToTop(dlg);
            return 1;
        }
        for (i = 0; i < dd_NumDevices; i++) {
            strcpy(buf, dd_Devices[i].desc);
            strcat(buf, " (");
            strcat(buf, dd_Devices[i].name);
            strcat(buf, ")");
            idx = SendMessage(dd_DrvCombo, CB_ADDSTRING, 0, (LPARAM)buf);
            SendMessage(dd_DrvCombo, CB_SETITEMDATA, idx, (LPARAM)dd_Devices[i].guid);
        }
        SendMessage(dd_DrvCombo, CB_SETCURSEL, 0, 0);
        SendMessage(dlg, WM_COMMAND, 102, (LPARAM)dd_DrvCombo);
        BringWindowToTop(dlg);
        return 1;
    case WM_CLOSE:
        cfg = reg_OpenKey("dd_vesa", 2);
        if (cfg) {
            if (!dd_DlgFlag) {
                i = SendMessage(dd_DrvCombo, CB_GETCURSEL, 0, 0);
                guid = (GUID *)SendMessage(dd_DrvCombo, CB_GETITEMDATA, i, 0);
                if (guid) {
                    if (s = dd_GuidToString(guid, guidstr))
                        reg_SetString(cfg, "dd_Guid", s);
                } else
                    reg_SetString(cfg, "dd_Guid", "00000000-0000-0000-0000000000000000");
            }
            i = SendMessage(dd_DevCombo, CB_GETCURSEL, 0, 0);
            guid = (GUID *)SendMessage(dd_DevCombo, CB_GETITEMDATA, i, 0);
            if (guid) {
                if (s = dd_GuidToString(guid, guidstr))
                    reg_SetString(cfg, "d3d_Guid", s);
            } else
                reg_SetString(cfg, "d3d_Guid", "00000000-0000-0000-0000000000000000");
            reg_CloseKey(cfg);
        }
        while (ShowCursor(FALSE) > 0)
            ;
        EndDialog(dlg, 0);
        return 1;
    }
    return 0;
}
