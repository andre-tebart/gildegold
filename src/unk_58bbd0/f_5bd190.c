/* 0x5bd190, a text control right-aligned by param_4 (96 B): the control 0x473650 (x - width), the visible
   flag +0x60 = 1 and the width +0x14, then 0x4760b0 (the id, param_5) */

void ai_Unk5bd190(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    int ctrl;

    ctrl = d2_CreateWinText(param_2 - param_4, param_3, param_1, U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x60) = 1;
    *(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x14) = (short)param_4;
    d2_SetObjFont(ctrl, param_5);
}
