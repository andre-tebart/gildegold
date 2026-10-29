/* 0x5bd1f0, a text control at a position (96 B): the control 0x473650 (x, y), the flag +0x60 = 0 and the
   width +0x14 (a word), then 0x4760b0 (the id, param_5) */

void ai_Unk5bd1f0(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    int ctrl;

    ctrl = d2_CreateWinText(param_2, param_3, param_1, U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x60) = 0;
    *(unsigned short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x14) = param_4;
    d2_SetObjFont(ctrl, param_5);
}
