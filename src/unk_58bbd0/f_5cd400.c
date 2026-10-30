/* 0x5cd400, the espionage result window "privillegien\gegenspionage3" 0x62d660 (1211 B, ROUGH
   SKELETON: no Ghidra export; from the disassembly): type 6 only (other types go to 0x5cd8b1). The
   frame 0x61c; 0x576040 (9, 0, 0, 0) and the window 0x4741f0 / 0x475600 / 0x475f70 (1); the caption
   0x1d36 through 0x515960 (the screen record 0xf8f120 + 0x6ed938 * 0x3c4, the words +8 / +0xa and the
   picture 0x6ed918 + 0x1d978); the record 0x143a97c and the branch 0x130 / 0x18f by the message byte
   +9. ROUGH SKELETON: the controls, the rows and the pump 0x5309a0 are not written. */

int ai_EspionageResult(int param_1, int *m)  /* m: the handler's second argument (unused here): the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int win;
    char text[0x100];

    if (*(char *)(param_1 + 2) != 6)
        return 0;   /* ROUGH: the other path at 0x5cd8b1 is not written */
    ai_Unk576040(9, 0, 0, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d660);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    io_PrintText(0x1d36);
    /* ROUGH SKELETON: the screen coordinates, the record 0x143a97c, the controls and the pump are not
       written yet */
    return 0;
}
