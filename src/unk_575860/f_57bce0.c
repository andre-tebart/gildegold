/* 0x57bce0, the tutorial chapter window (541 B): the window 0x4741f0 (0, 0, "tutorial\CHOOSE_tutorial" 0x62a248)
   with 0x475600, 0x475f70 (id, 1), the text 0x3f1a through 0x515960 (0x6024f8) and 0x475f70 (id, 2); the five
   chapter entries (i = 0 .. 4): the text 0x3f1b + i through 0x515960 (0x6024f8) into 0x475b80 (id, 2, text), the
   id stored in the local list, and in the window table entry 0xf8f080 + id * 0x2ec: +0x48 (checkable) = 1,
   +0x28 (checked) = the chapter table 0x62918c[i], +0x1c0 (style) &= -3; the two extra texts 0x3f20 (the start)
   / 0x3f21 (the back) through 0x475b80; control 2 via 0x475f70. Then the frame pump 0x5309a0 (this function,
   0x86), every returned frame: the five entries scanned for a checked one (their +0x28 != 0) and 0x475e40 (the
   start, 1), or 0x475e40 (the start, 0) when none is set; the escape (the right button 0x14673a0, the key
   0x1468410 == 1, the enter 0xf8a668 == 1) sets 0x6f22a0 = 1; a click (0xf8a668 != -1) on the back sets
   0x6f22a0 = 1, on the start sets 0x6f22a0 = 1, writes the checked state of the five entries back to 0x62918c
   and makes the result 1 when at least one is checked. 0x4758f0 closes the window; 1 starts the tutorial, 0 is
   the back. (The menu that is skipped when this function pops at once is exactly this pump loop.) */

int sv_RunFrame(int cb, unsigned mask);

int ai_Unk57bce0(void)
{
    int start;
    int result;
    int back;
    int ids[5];
    int i;
    int n;
    int id;

    result = 0;
    id = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a248);
    d2_CentreViewWindows(id);
    d2_SetForm(id, 1);
    io_PrintText((char *)(unsigned)"$Z%s", 0x3f1a);
    d2_SetForm(id, 2);
    for (i = 0; i < 5; i++) {
        ids[i] = d2_GetWinObj(id, 2, io_PrintText((char *)(unsigned)"%ia[%s]$N", 0x3f1b + i));
        U_I(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec + 0x48) = 1;
        U_I(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec + 0x28) = U_I((unsigned)&full_data_62918c + i * 4);
        U_I(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec + 0x1c0) &= -3;
    }
    start = d2_GetWinObj(id, 2, io_PrintText((char *)(unsigned)"%ia[%s]$N", 0x3f20));
    back = d2_GetWinObj(id, 2, io_PrintText((char *)(unsigned)"%ia[%s]$N", 0x3f21));
    d2_SetForm(id, 2);
    while (sv_RunFrame((unsigned)&ai_Unk57bce0, 0x86) != 0) {
#ifdef REA_START
        /* REA_START=tutorial (`uv run play|dbg --start tutorial`): the chapter menu is the screen between the
           main-menu entry and the loading screen. Accept its default selection (0x62918c starts all 1) the way
           the "Tutorial starten" click does, so a run without GUI automation still reaches the tutorial;
           without the flag the menu waits for a real click. The frame above has drawn and registered it. */
        {
            char *rea = getenv("REA_START");
            if (rea != 0 && strcmp(rea, "tutorial") == 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                result = 1;
            }
        }
#endif
        n = 0;
        for (i = 0; i < 5; i++) {
            if (U_I((char *)(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec) + 0x28) != 0) {
                d2_SetObjEnabled(start, 1);
                break;
            }
            n++;
        }
        if (n == 5)
            d2_SetObjEnabled(start, 0);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(unsigned char *)(unsigned)&kb_Last == 1 || U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == back) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == start) {
                U_I((unsigned)&sv_PopRequest) = 1;
                n = 0;
                for (i = 0; i < 5; i++) {
                    if (U_I((char *)(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec) + 0x28) != 0) {
                        n++;
                        U_I((unsigned)&full_data_62918c + i * 4) = 1;
                    } else {
                        U_I((unsigned)&full_data_62918c + i * 4) = 0;
                    }
                }
                if (n != 0)
                    result = 1;
            }
        }
    }
    d2_CloseView(id);
    return result;
}
