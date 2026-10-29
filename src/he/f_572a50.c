/* 0x572a50, the run of the event 0x92: state -2 releases the window +0x74 and frees; state 0 opens the window 0x53d6f0 (0x6269c0, 0x5ec), 0x475600 / 0x475f70, the texts 0x602568 and 0x411c, 0x4c27d0 (-7, 0, 0x628fe4, -1, 0) and state 1; state 1: due (0x4fe210) -> 0x5729c0, 0x53d800 and free; the window missing -> state 0; the actor 0xf8aac4 == window +8 and 0xf8a668: 0 -> 0x5728d0, 1 -> 0x5729c0, then 0x53d800 and free */

void he_Unk572a50(int ev)
{
    int state = HM_I(ev, 0x70);

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
        io_PrintText(0x411c, HM_I(ev, 0xb8));
        snd_QueueVoice(-7, 0, (char *)(unsigned)&full_data_628fe4, -1, 0);
        HM_I(ev, 0x70) = 1;
    } else if (state == 1) {
        if (gm_CompareTime(&ai_Time, ev + 0x52) > 0) {
            he_Unk5729c0(ev);
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (HM_I(ev, 0x74) == 0) {
            HM_I(ev, 0x70) = 0;
            return;
        }
        if (*(int *)(unsigned)&d2_Unkf8aac4 == HM_I(HM_I(ev, 0x74), 8) && *(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                he_Unk5728d0(ev);
                he_Unk53d800(ev);
                he_Unk53e1f0((int *)ev);
                return;
            }
            if (*(int *)(unsigned)&d2_ClickedId == 1) {
                he_Unk5729c0(ev);
                he_Unk53d800(ev);
                he_Unk53e1f0((int *)ev);
                return;
            }
        }
    }
}
