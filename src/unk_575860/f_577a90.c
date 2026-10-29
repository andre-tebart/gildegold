/* 0x577a90, no return value: nothing without the run record or its text +0x14; else the text window S+0x24 is
   rewritten (0x52e330, control 0 with the text 0x602568 and the record text +0x14, control 1 with 0x475590) and,
   when the record word +0x18 is set, the window S+0x14 recreated by 0x4c2760 (0xfffffff9, 0, +0x18, -1) after
   releasing the previous one (0x426f20 / 0x427290) */

void ai_Unk577a90(void)
{
    char *st;
    char *r;

    st = S;
    r = *(char **)(st + 0x18);
    if (!r)
        return;
    if (!*(int *)(r + 0x14))
        return;
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    d2_SetForm(U_I(S + 0x24), 0);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0x14));
    d2_SetViewEnabled(U_I(S + 0x24), 1);
    st = S;
    if (!*(int *)(*(char **)(st + 0x18) + 0x18))
        return;
    if (U_I(st + 0x14)) {
        if (sb_IsPlaying(U_I(st + 0x14)))
            sb_Stop(U_I(st + 0x14), 0);
        U_I(st + 0x14) = 0;
    }
    r = *(char **)(S + 0x18);
    U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, *(int *)(r + 0x18), -1);
}
