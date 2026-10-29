/* 0x56fc50, the alloc of the event 0x88 (a work at the building +0xb0 of the player +8): paused -> nothing; with a running event 0x88 of the same player and building the text 0x4096 and state -1; else the due clock now + 1 hour and state 0 (0x4c4c60), or -1 when +0xb4 is -1, and the message 0x50cc50 (pid, -(+0xb8)) */

void he_Unk56fc50(int ev)
{
    char text[0x400];
    int r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    for (r = (int)he_FindEvent(1, 0, 0x88); r != 0; r = (int)he_NextEvent()) {
        if (r != ev && *(unsigned short *)(r + 8) == *(unsigned short *)(ev + 8) && HM_I(r, 0xb0) == HM_I(ev, 0xb0))
            break;
    }
    if (r != 0) {
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        txt_Format(text, 0x4096, HM_I(ev, 0xb0));
        he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fa, 0);
        return;
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (HM_I(ev, 0xb4) == -1)
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
    else {
        HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
        gm_Unk50cc50(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -HM_I(ev, 0xb8));
    }
}
