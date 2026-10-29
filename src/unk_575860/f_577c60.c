/* 0x577c60, clears the second text window S+0x34 (106 B): control 0 off (0x475f70), the text 0x602568
   (0x515960), 0x475590 (S+0x34, 0); then the window S+0x14 is released (0x426f20 / 0x427290) and cleared */

void ai_Unk577c60(void)
{
    d2_SetForm(U_I(S + 0x34), 0);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetViewEnabled(U_I(S + 0x34), 0);
    if (U_I(S + 0x14)) {
        if (sb_IsPlaying(U_I(S + 0x14)))
            sb_Stop(U_I(S + 0x14), 0);
        U_I(S + 0x14) = 0;
    }
}
