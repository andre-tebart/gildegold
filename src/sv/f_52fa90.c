/* 0x52fa90, destroys the scroll form */

void sv_Unk52fa90(void)
{
    if (ai_Unk623ea0 != -1) {
        d2_CloseView(ai_Unk623ea0);
        ai_Unk623ea0 = -1;
        ai_Unk623ea4 = -1;
        ai_Unk623ea8 = -1;
    }
}
