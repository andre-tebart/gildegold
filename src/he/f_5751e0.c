/* 0x5751e0, the run of the event 0xa3: -2 / -1 free, also above the state 3 and without the figure +0xc (0x505df0); state + 1, the
   due clock + 3 + random (3) hours (0x4fe140); from the figure's +0x194 a payment -x (x = random (2) + 1, drawn again when below
   +0x194, 0x50cc50) and for the kinds 6 / 7 the text 0x1008 and the message 0x5fb (0x53dda0); when the state passes 3 the due
   clock +0x52 = now */

void he_Unk5751e0(int ev)
{
    char text[0x400];
    char *per;
    int n, x, state;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (state > 3) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (per == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    HM_I(ev, 0x70) = state + 1;
    gm_AddTime(ev + 0x52, (util_Random(3) & 0xffff) + 3, 0, 0);
    n = HM_I(per, 0x194);
    if (n >= 1) {
        x = (util_Random(2) & 0xffff) + 1;
        if (x < n)
            x = (util_Random(2) & 0xffff) + 1;
        gm_Unk50cc50(HM_I(per, 4), -x);
        if (HM_B(per, 2) == 6 || HM_B(per, 2) == 7) {
            txt_Format(text, 0x1008, x);
            he_ShowNews(HM_I(per, 4), -1, text, 0, 0x5fb, 0);
        }
    }
    if (HM_I(ev, 0x70) > 3)
        *(HETIME *)(ev + 0x52) = ai_Time;
}
