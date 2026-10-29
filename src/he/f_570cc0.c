/* 0x570cc0, the run of the event 0x8b (a fight / attack of the character +0xac on the player +8): a missing character or a player without the bit 1 of +0x770d4c -> state -2; -2 releases the window +0x74 (0x53d800) and frees; state 0 opens the window 0x53d6f0 (0x6269c0, 0x5ec), 0x475600 / 0x475f70, the texts 0x602568 and 0x40fd and 0x4c27d0 (-7, .., 0x628ecc, -1, 0), state 1; state 1: due (0x4fe210) -> text 0x40ff (0x570c70), 0x53d800, free; the window missing -> state 0; the actor 0xf8aac4 != window +8 or 0xf8a668 -1 -> nothing; 0xf8a668 0 -> 0x570bf0; 1 -> 0x570c70, then 0x53d800 and free. ROUGH */

int he_Unk53d6f0();                 /* 0x53d6f0 */

void he_Unk570cc0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xac));
    int state;

    if (per == 0 || (HM_B(((unsigned)&ai_Selected + 0x44) + *(unsigned short *)(ev + 8) * 0x21c, 0) & 1) == 0)
        HM_I(ev, 0x70) = -2;
    state = HM_I(ev, 0x70);
    if (state == -2) {
        if (HM_I(ev, 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (state == 0) {
        if (HM_I(ev, 0x74) == 0)
            he_Unk53d6f0(ev, 0, 0, (char *)((unsigned)&ai_Unk625820 + 0x11a0), 0x5ec);
        if (HM_I(ev, 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_CentreViewWindows(HM_I(HM_I(ev, 0x74), 8));
        d2_SetForm(HM_I(HM_I(ev, 0x74), 8), 0);
        io_PrintText((unsigned)"$C");
        io_PrintText(0x40fd, *(unsigned short *)per, HM_I(ev, 0xb8));
        snd_QueueVoice(-7, 0, (char *)(unsigned)&full_data_628ecc, -1, 0);
        HM_I(ev, 0x70) = 1;
        return;
    }
    if (state != 1)
        return;
    if (gm_CompareTime(&ai_Time, ev + 0x52) > 0) {
        he_Unk570c70((int *)(ev + 0xac), (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (HM_I(ev, 0x74) == 0) {
        HM_I(ev, 0x70) = 0;
        return;
    }
    if (*(int *)(unsigned)&d2_Unkf8aac4 != HM_I(HM_I(ev, 0x74), 8))
        return;
    state = *(int *)(unsigned)&d2_ClickedId;
    if (state == -1)
        return;
    if (state == 0) {
        he_Unk570bf0((int *)(ev + 0xac), (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (state != 1)
        return;
    he_Unk570c70((int *)(ev + 0xac), (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    he_Unk53d800(ev);
    he_Unk53e1f0((int *)ev);
}
