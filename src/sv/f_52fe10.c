/* 0x52fe10, the chat console form "misc\chatconsole" (also the message loop callback of 0x5309a0 (0x52fe10, 0x657c7)): one button per human player (the characters 0x770bd4 stride 0x21c with kind 7 at -0x52;
   the buttons remember their state in 0x623ebc), the input line (0x475b80 form text) with the "$<n>FF " colour prefix and the player's text, sent through 0x4c5aa0; ROUGH */

int sv_Unk52fe10(void)
{
    int ids[16];
    char line[0x60];
    int i, f, ed, w, *p, r, k;
    char *s;

    for (i = 0; i < 8; i++)
        ids[i] = -1;
    ai_Unk623ec4 = 0;
    f = d2_OpenForm(0, 0, "misc\\chatconsole");
    d2_PlaceMainWindow(f, 3);
    d2_SetForm(f, 3);
    i = 0;
    p = (int *)((unsigned)&ai_Unk770b89 + 0x4b);
    do {
        if (*(char *)((int)p - 0x52) == 7) {
            w = d2_WinOpenPic(i * 0x30 + 100, 4, *p, d2_LastWindow);
            ids[i] = w;
            SV_WI(w, 0x48) = 1;
            SV_WI(w, 0x2e8) = (int)(p - 0x15);
            d2_SetObjValue(w, *(char *)((unsigned)&full_data_623ebc + i));
            i++;
        }
        p += 0x87;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk770b89 + 0x653f9) + 0x52));
    w = d2_WinCreateSlider(0, 0, 0xc5, SV_I((unsigned)&full_data_14414c8), d2_LastWindow);
    d2_SetObjFont(w, 0x54);
    d2_SetForm(f, 2);
    ed = d2_GetWinObj(f, 2, io_PrintText(0x1cc3, ch_EmptyName));
    d2_SetObjMax(ed, 0x60);
    r = sv_RunFrame(sv_Unk52fe10, 0x657c7);
    do {
        if (r == 0) {
            for (i = 0; i < 8; i++) {
                if (ids[i] != -1)
                    *(char *)((unsigned)&full_data_623ebc + i) = d2_GetObjValue(ids[i]);
            }
            d2_CloseView(f);
            *(char *)(unsigned)&kb_Last = 0;
            return 0;
        }
        d2_SetForm(f, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText((char *)(unsigned)&ai_Unk75fba0);
        if (SV_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1)
            SV_I((unsigned)&sv_PopRequest) = 1;
        d2_FocusInput(ed);
        if ((SV_I((unsigned)&d2_ClickedId) != -1 || *(char *)(unsigned)&kb_Last == 0x1c) && (ai_Unk5fe710 == w || *(char *)(unsigned)&kb_Last == 0x1c)) {
            for (k = 0; k < 8; k++) {
                if (ids[k] != -1 && d2_GetObjValue(ids[k]) != 0)
                    ids[k + 8] = *(int *)(*(int *)(SV_I((unsigned)&d2_Objs) + 0x2e8 + ids[k] * 0x2ec) + 4);
            }
            sprintf(line, "$%iFF ", SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c) - 0x59a);
            s = (char *)d2_GetObjValue(ed);
            strcat(line, s);
            strcat(line, "$A");
            d2_SetObjValue(ed, ch_EmptyName);
            ai_Unk4c5aa0(ids + 8);
            SV_I((unsigned)&sv_PopRequest) = 1;
        }
        r = sv_RunFrame(sv_Unk52fe10, 0x657c7);
    } while (1);
}
