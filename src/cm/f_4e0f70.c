/* 0x4e0f70, a birth (the message m: +0x34 the mother, +0x38 the child; +0x30 count): the scene "Geburt.ed3" with "cutscenes\\geburt\\enter_Geburt.esc",
   the screams "_GEBURT_GESCHREI" (2 .. 5 of them, random pauses), the view "cutscenes\\geburt" with the text 0x1aae (a boy) / 0x1aaf and
   the name box ("$L%t" + the child's name), waits for a key (0x1468410 == 0x1c) or the end of the frame wait, then the field
   blocks (0x4c3ba0 / 0x4c3c00) for the new person */

int ai_Unk4e0f70(char *m)
{
    char name[0x20];
    char *a, *b, *t;
    int snd, sc, v, w, n;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0)
        return 0;
    n = 8;
    cm_BeginFieldBlock(b, *(int *)(b + 4));
    cm_AddFieldDelta(1, 1, 2, &n);
    cm_SendFieldDeltas();
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    snd = ai_Unk4e3270("GEBURT.sbf", 0);
    ai_Unk4e3620("cd2\\Geburt.mp3");
    ai_Unk4e2e60("Geburt.ed3");
    sc = ai_Unk4e2bf0("cutscenes\\geburt\\enter_Geburt.esc");
    if (ai_Unk4e3490(0x1964, 0, 0) == 0) {
        if (a[9] == 0)
            n = ai_Unk4e52f0(2) + (b[9] == 0 ? 0 : 2);
        else
            n = ai_Unk4e52f0(2) + (b[9] == 0 ? 4 : 6);
        ai_Unk4e32d0(-8, "_GEBURT", n, 0);
        if (ai_Unk4e3490(0x3e8, 0, 0) == 0) {
            for (n = ai_Unk4e52f0(4) + 2; n > 0; n--)
                ai_Unk4e32d0(-8, "_GEBURT_GESCHREI", ai_Unk4e52f0(5), (unsigned short)util_Random(0x800) + 0x3e8);
            ai_Unk4e3490(0x1b58, 0, 0);
        }
    }
    v = d2_OpenForm(0, 0, "cutscenes\\geburt");
    d2_CentreViewWindows(v);
    d2_SetForm(v, 1);
    io_PrintText(b[9] == 0 ? 0x1aae : 0x1aaf);
    d2_SetForm(v, 2);
    w = d2_GetWinObj(v, 2, io_PrintText("$L%t", b + 0x30));
    d2_SetObjMax(w, 0xf);
    d2_FocusInput(w);
    do {
        if (kb_Last == 0x1c || d2_FocusedInput != 0)
            break;
        d2_GetObjValue(w);
        strcpy(name, (char *)d2_GetObjValue(w));
        if (name[0] != 0)
            break;
        d2_FocusInput(w);
    } while (sv_RunFrame(0, ai_Unk60d360) != 0 && ai_Unk6f2438 == 0);
    cm_BeginFieldBlock(b, *(int *)(b + 4));
    cm_AddFieldDelta(1, 0x10, 0x30, &n);
    cm_SendFieldDeltas();
    d2_CloseView(v);
    ai_Unk4e3060();
    ai_Unk4e2d90(sc);
    ai_Unk4e2c70(sc);
    snd_ClearQueue();
    ai_Unk4e3290(snd);
    ai_Unk6f0da4 = 0;
    ai_Unk4e30c0();
    ai_Unk4e3640();
    d2_CutsceneActive = 0;
    return 1;
}
