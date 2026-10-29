/* 0x594810, the bribe window "misc\Bestechung" (759 B, ROUGH SKELETON from the disassembly; an unnamed Ghidra
   start): rec (0 or +0x27 == 0xffff returns); 0x553cc0 (rec) != 0 gives the text 0x1a92 (0x5176d0 /
   0x521150); else the kind record 0x7e3f30 + rec[0] * 0x24d, the acting building 0x6f2308 / 0x6f230c, its
   records 0x4ffe90 (+0x5d, 1, 0, 0xca) / 0x4ffcc0 (the +0x2a != rec + 1 collected), the window 0x4741f0 /
   0x475600 / 0x476f80 / 0x475f70, the text 0x515960 and the controls 0x475b80, the messages 0x4c4b70 /
   0x4c2d20 / 0x52ae40, the pump 0x5309a0 (this, 0x657c7) and the exit 0x4758f0 */

unsigned short * gm_QueryNext(void); void txt_Format(char *out, int id0, ...);

int ai_Unk594810(int param_1, int rec)
{
    char buf[0x200];
    int list[0x80];
    int win;
    int ctl1;
    int ctl2;
    int act;
    int b;
    int i;
    int n;
    int kind;

    if (rec == 0 || *(unsigned short *)(rec + 0x27) == 0xffff)
        return 0;
    if (he_Unk553cc0(rec) != 0) {
        txt_Format(buf, 0x1a92, *(unsigned short *)(rec + 0x27), rec);
        sv_MessageBox(buf, param_1);
        return 0;
    }
    kind = *(unsigned char *)rec;
    act = U_I((unsigned)&ai_PlayerBuilding) != 0 ? U_I((unsigned)&ai_PlayerBuilding) : U_I((unsigned)&ai_FallbackBuilding);
    n = 0;
    if (act != 0) {
        for (b = gm_Query(*(int *)(act + 0x5d), 1, 0, 0xca); b != 0; b = gm_QueryNext()) {
            if (*(int *)(b + 0x2a) != U_I(rec + 1)) {
                list[n] = b;
                n++;
            }
        }
    }
    win = d2_OpenForm(0, 0, "misc\\Bestechung");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    i = io_PrintText(0x1a92);
    ctl1 = d2_GetWinObj(win, 1, i);
    ctl2 = d2_GetWinObj(win, 1, i + 1);
    /* ROUGH: the selection over the collected records, the offer / money texts (0x4c4b70 / 0x4c2d20), the
       confirmation 0x52ae40, the frame pump 0x5309a0 (this, 0x657c7) and the exit 0x4758f0 (win) */
    while (sv_RunFrame(ai_Unk594810, 0x657c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    return 0;
}
