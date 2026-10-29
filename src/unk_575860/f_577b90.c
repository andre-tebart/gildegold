/* 0x577b90, the sibling of 0x577a90 for the second text window S+0x34 (203 B): nothing without the run record
   or its second text +0x1c; else the window S+0x34 is rewritten (0x52e330, control 0 with the text 0x602568 and
   the record text +0x1c, 0x475590 control 1) and, when the record word +0x20 is set, the window S+0x14 recreated
   by 0x4c2760 (0xfffffff9, 0, +0x20, -1) after releasing the previous one (0x426f20 / 0x427290) */

void ai_Unk577b90(void)
{
    char *r;

    if (!*(int *)(S + 0x18))
        return;
    if (!*(int *)(*(char **)(S + 0x18) + 0x1c))
        return;
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    d2_SetForm(U_I(S + 0x34), 0);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0x1c));
    d2_SetViewEnabled(U_I(S + 0x34), 1);
    if (!*(int *)(*(char **)(S + 0x18) + 0x20))
        return;
    if (U_I(S + 0x14)) {
        if (sb_IsPlaying(U_I(S + 0x14)))
            sb_Stop(U_I(S + 0x14), 0);
        U_I(S + 0x14) = 0;
    }
    r = *(char **)(S + 0x18);
    U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, *(int *)(r + 0x20), -1);
}
