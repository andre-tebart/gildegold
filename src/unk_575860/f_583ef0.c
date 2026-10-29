/* 0x583ef0, enables the graveyard buttons by the state record (277 B): rec = the record (its +0x65 the state
   record), from = the last drawn index, btns = the four button ids, sel = the selected one. The buttons 0 ..
   from disabled (0x475e40 (the button, 0) and the window table +0x28 = 0); the buttons from .. 3: 0x4ef4f0 (the
   state record, i) -> 0 enabled / 0 (0x475e40 (the button, 0) and +0x28 = 0), else enabled and the first one
   kept; with the selected button marked in the table (+0x3c) and some button enabled, the first enabled
   becomes the selection and is marked at +0x28; 1 when any button is enabled, else 0 */

int ai_Unk583ef0(char *rec, int from, int unused, int *btns, int *sel)
{
    int i;
    int any;
    int r;

    from++;
    any = -1;
    for (i = 0; i < from; i++) {
        d2_SetObjEnabled(btns[i], 0);
        *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[i] * 0x2ec) = 0;
    }
    r = 0;
    for (i = from; i < 4; i++) {
        if (unk_Unk4ef4f0(U_I(rec + 0x65), i) == 0) {
            d2_SetObjEnabled(btns[i], 0);
            *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[i] * 0x2ec) = 0;
        } else {
            r = 1;
            d2_SetObjEnabled(btns[i], 1);
            if (any == -1)
                any = i;
        }
    }
    if (*(int *)(U_I((unsigned)&d2_Objs) + 0x3c + btns[*sel] * 0x2ec) != 0 && any != -1) {
        *sel = any;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[any] * 0x2ec) = 1;
    }
    return r;
}
