/* 0x5bd0c0, a text control of width param_4 on a screen (128 B): 0x476030 (param_5) first, then, when the
   screen record 0xf8f148 + screen * 0x3c4 is set, the background 0x46f1a0 and the clip 0x476760; the control
   0x473650 (centred) gets the visible flag +0x5c and the width +0x14, then 0x4760b0 (the id, param_5) */

void ai_Unk5bd0c0(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    int ctrl;

    d2_SetFont(param_5);
    if (U_I(((unsigned)&ai_Unkf8f13c + 0xc) + U_I((unsigned)&d2_LastWindow) * 0x3c4) != 0) {
        d2_DrawTextBox(param_2 - param_4 / 2, param_3, param_4, 0x140, 0, param_1, 8);
        d2_FillPaintbox(U_I((unsigned)&d2_TextLeft), U_I((unsigned)&d2_TextTop), U_I((unsigned)&d2_TextDrawnW), U_I((unsigned)&d2_TextDrawnH));
    }
    ctrl = d2_CreateWinText(param_2 - param_4 / 2, param_3, param_1, U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x5c) = 1;
    *(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x14) = (short)param_4;
    d2_SetObjFont(ctrl, param_5);
}
