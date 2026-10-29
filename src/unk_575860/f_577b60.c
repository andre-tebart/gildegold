/* 0x577b60, clears the text window S+0x24: control 0 off (0x475f70), the text 0x602568 (0x515960), 0x475590
   (S+0x24, 0); no return value */

void ai_Unk577b60(void)
{
    d2_SetForm(U_I(S + 0x24), 0);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetViewEnabled(U_I(S + 0x24), 0);
}
