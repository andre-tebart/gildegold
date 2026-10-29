/* 0x5307f0, the "advance game" callback / screen (form "misc\advancegame", 0x6f3608 = 0x2ee, 0x5f1290 = 4.9375 the game speed): runs the message loop 0x5309a0 (0x5307f0, mask) while it returns non zero and
   restores the speed (0x6f3608 * 0.00625 + 0.25) afterwards */

void sv_Unk5307f0(void)
{
    unsigned mask;
    int sp, r, f;

    mask = SV_U((unsigned)&sv_FrameMask);
    sp = SV_I((unsigned)&ai_OptionSpeed);
    if (ai_Unk5763c0(8) != 0) {
        ai_Unk576040(0xb, 0, 0, 0);
        mask = mask & 0xffffc3e7 | 0xc0;
        SV_I((unsigned)&full_data_6f22ac) = 1;
        SV_I((unsigned)&ai_OptionSpeed) = 0x2ee;
        *(float *)(unsigned)&ch_Unk5f1290 = 4.9375f;
        f = d2_OpenForm(0, 0, "misc\\advancegame");
        d2_CentreViewWindows(f);
        io_PrintText(0x82);
        r = sv_RunFrame(sv_Unk5307f0, mask);
        while (r != 0) {
            if (*(char *)(unsigned)&kb_Last != 0 || SV_I(((unsigned)&dm_L + 0xc)) != 0 || SV_I(((unsigned)&dm_L + 0x20)) != 0)
                SV_I((unsigned)&sv_PopRequest) = 1;
            r = sv_RunFrame(sv_Unk5307f0, mask);
        }
        SV_I((unsigned)&full_data_6f22ac) = 0;
        *(float *)(unsigned)&ch_Unk5f1290 = (float)sp * 0.00625f + 0.25f;
        SV_I((unsigned)&ai_OptionSpeed) = sp;
        d2_CloseView(f);
        ai_Unk576040(0xb, 0, 1, 0);
    }
}
