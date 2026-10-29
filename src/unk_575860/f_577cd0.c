/* 0x577cd0, creates the tutorial progress form ("tutorial\progress" 0x629314, 129 B): the form id
   0x4741f0 (0, 0, name) into S+0x68, its controls (0x475850 (id, 3), 0x475f70 (id, 0)) and the slider
   S+0x6c = 0x46a960 (x, 5, 400, 0, 400, 0x75, 0x42, d2_LastWindow), x = the short +0 of the window record
   0xf8f128 + i * 0x3c4 (i = d2_LastWindow) / 2 - 200 (centered on the 400 wide slider) */

void ai_Unk577cd0(void)
{
    int i;

    U_I(S + 0x68) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629314);
    d2_PlaceMainWindow(U_I(S + 0x68), 3);
    d2_SetForm(U_I(S + 0x68), 0);
    i = U_I((unsigned)&d2_LastWindow);
    U_I(S + 0x6c) = d2_WinOpenSlider(*(short *)((unsigned)&ai_Unkf8f128 + i * 0x3c4) / 2 - 0xc8, 5, 0x190, 0, 0x190,
                                 0x75, 0x42, i);
}
