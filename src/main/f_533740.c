/* 0x533740, the money amount dialog (a number input 0x472390 (0x20, 0x60, 0x26c, 400, 0x15) with the text "$3ABetrag:%m", range 0 .. 10000): runs 0x5309a0 until closed */

void main_Unk533740(void)
{
    int dlg, ed, run;

    U_I((unsigned)&ai_Unkfb9068) = 0;
    U_I((unsigned)&ai_Unkfb9060) = 1;
    U_I((unsigned)&ai_Unkfb9064) = 1;
    U_I((unsigned)&ai_Unkfb906c) = 2;
    dlg = d2_CreateWindow(0x20, 0x60, 0x26c, 400, 0x15);
    ed = d2_GetWinObj(-1, dlg, io_PrintText("$3ABetrag:%m"));
    d2_SetObjValue(ed, 0, 10000, 0);
    run = sv_RunFrame((int)main_Unk533740, 0x24087);
    while (run != 0) {
        if (*(char *)(unsigned)&kb_Last == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        run = sv_RunFrame((int)main_Unk533740, 0x24087);
    }
    d2_RemoveWindow(dlg);
}
