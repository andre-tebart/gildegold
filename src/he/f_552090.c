/* 0x552090, the run handler of the master exam style event "Berufe\Messagebox_Meisterpruefung" (ROUGH, same shape as 0x547430): states -1 / -2 free the record (and its window), state 0 opens the window, shows the page 0x602568 and the news 0x10b9 (player name, figure +8), state 1 waits for the due time (then it closes) or the answer (0xf8a668: 0 = the figure's record 0x53e400 (2, 2, id, 1, +0xb0) gets +0xb0 = 1, 1 = nothing) and closes */

void he_Unk552090(int ev)
{
    int st = *(int *)(ev + 0x70);
    int h;
    char *r;

    if (st == -1 || st == -2) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        if (*(int *)(ev + 0x74) == 0)
            he_Unk53d6f0(ev, 0, 0, (int)"Berufe\Messagebox_Meisterpruefung", 0x5ec);
        if (*(int *)(ev + 0x74) != 0) {
            d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
            d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
            io_PrintText("$C");
            io_PrintText(0x10b9, (char *)((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4, *(unsigned short *)(ev + 8));
            *(int *)(ev + 0x70) = 1;
        } else {
            he_Unk53e1f0((int *)ev);
        }
        return;
    }
    if (st == 1) {
        if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8))
            return;
        h = U_I((unsigned)&d2_ClickedId);
        if (h == -1)
            return;
        if (h == 0) {
            r = he_FindEvent(2, 2, *(unsigned short *)(ev + 8), 1, *(int *)(ev + 0xb0));
            *(int *)(r + 0xb0) = 1;
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
        } else if (h == 1) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
        }
    }
}
