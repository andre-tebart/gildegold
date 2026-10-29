/* 0x570130, the alloc of the event 0x89 (a work at +0xb0 by the character +0xb4): paused -> nothing; with a running event 0x89 of the same player and building the text 0x40a2, state -1 and free; else the character +0xb4 missing -> free; the due clock now + 1 hour, 0x50cc50 (pid, -(+0xb8)), state 0 and 0x4f2e30 (0x1b, 1, pid, +4 of the character, the id +1 of +0x170 or -1) */

void he_Unk570130(int ev)
{
    char text[0x400];
    char *per;
    int r, id;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xb0));
    for (r = (int)he_FindEvent(1, 0, 0x89); r != 0; r = (int)he_NextEvent()) {
        if (r != ev && *(unsigned short *)(r + 8) == *(unsigned short *)(ev + 8) && HM_I(r, 0xb0) == HM_I(ev, 0xb0))
            break;
    }
    if (r != 0) {
        if (per != 0) {
            txt_Format(text, 0x40a2, *(unsigned short *)per);
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fa, 0);
        }
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_FindChar(HM_I(ev, 0xb4)) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    HM_I(ev, 0xbc) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    gm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -HM_I(ev, 0xb8));
    HM_I(ev, 0x70) = 0;
    if (per != 0 && HM_I(per, 0x170) != 0)
        id = *(int *)(HM_I(per, 0x170) + 1);
    else
        id = -1;
    if (per != 0)
        r = HM_I(per, 4);
    else
        r = -1;
    unk_Unk4f2e30(0x1b, 1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), r, id);
}
