/* 0x5337f0, creates a menu button (0x46bf10 (0x20, n * 0x2b + 10, 0xbf, text, form), shown flag +0x5c, width 300) for the row n of a dialog */

int main_Unk5337f0(int n, int text, int form)
{
    int b;

    b = d2_WinCreateSlider(0x20, n * 0x2b + 10, 0xbf, text, form);
    U_I(U_I((unsigned)&d2_Objs) + 0x5c + b * 0x2ec) = 1;
    d2_SetObjWidth(b, 300);
    return b;
}
