/* 0x532c30, the "Menu\CHOOSENETWORK_IP" dialog: asks for the host address (the INI value [Network] Host, default 127.0.0.1), stores the entered address in 0x75e1c0 and in the INI file 0x75e480; returns 1 when
   confirmed */

int main_Unk532c30(void)
{
    char buf[260];
    int r, f, ed, ok, run, i;
    char *s;
    char c;

    r = 0;
    GetPrivateProfileStringA("Network", "Host", (LPCSTR)(unsigned)&full_data_624b48, buf, 0x104, (char *)(unsigned)&ai_OptionsFile);
    f = d2_OpenForm(0, 0, "Menu\\CHOOSENETWORK_IP");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 2);
    io_PrintText(0x1cae);
    d2_SetForm(f, 0);
    ed = d2_GetWinObj(f, 0, 0);
    d2_SetObjValue(ed, buf);
    d2_SetForm(f, 1);
    ok = d2_GetWinObj(f, 1, io_PrintText(0x1cb0));
    run = sv_RunFrame((int)main_Unk532c30, 0x24087);
    while (run != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if ((U_I((unsigned)&d2_ClickedId) != -1 || *(char *)(unsigned)&kb_Last == 0x1c) && (ai_Unk5fe710 == ok || *(char *)(unsigned)&kb_Last == 0x1c)) {
            r = 1;
            s = (char *)d2_GetObjValue(ed);
            strcpy(buf, s);
            i = 0;
            do {
                c = buf[i];
                *(char *)((unsigned)&full_data_75e1c0 + i) = c;
                i++;
            } while (c != 0);
            WritePrivateProfileStringA("Network", "Host", buf, (char *)(unsigned)&ai_OptionsFile);
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        run = sv_RunFrame((int)main_Unk532c30, 0x24087);
    }
    d2_CloseView(f);
    return r;
}
