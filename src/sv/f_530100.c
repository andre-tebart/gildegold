/* 0x530100, shows / hides the quick chat strip "misc\quickchat" (0x623ec8): visible while a chat line is fresh (0x623ec4 + 0x23a ms) and the console 0x52fe10 is not the top of the 0x75f780 stack */

void sv_Unk530100(void)
{
    if (SV_I((unsigned)&ai_Unk75f280 + 0x500 + SV_I((unsigned)&sv_FrameDepth) * 8) == (int)sv_Unk52fe10
            || SV_U((unsigned)&ai_Unk623ec4) <= SV_U((unsigned)&ai_Unk6333cc) - 0x23a
            || SV_U((unsigned)&ai_Unk623ec4) == 0xffffffff) {
        if (ai_Unk623ec8 != -1) {
            d2_CloseView(ai_Unk623ec8);
            ai_Unk623ec8 = -1;
            return;
        }
    } else {
        if (ai_Unk623ec8 == -1) {
            ai_Unk623ec8 = d2_OpenForm(0, 0, "misc\\quickchat");
            d2_PlaceMainWindow(ai_Unk623ec8, 3);
            if (ai_Unk623ec8 == -1)
                return;
        }
        d2_SetForm(ai_Unk623ec8, 0);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText((char *)(unsigned)&ai_Unk75fba0);
    }
}
