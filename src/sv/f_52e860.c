/* 0x52e860, builds the slider of the form f at (x, y): the two bars (pictures 3 / 4, widgets *a / *b shown, kind 3, +0x1e0 = form index), the grip button (0x473650, picture 0x54) in *c; stores them in the form
   record (0xf8f4c8 ..) and the callback p5 in the form record of g (+0x384) */

int sv_CreateFormSlider(int x, int y, int f, int g, int p5, int *b, int *a, int *c)
{
    int id, fr;

    id = d2_WinOpenPic(x, y, 3, f);
    *a = id;
    fr = f * 0x3c4;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x12c) + fr) = id;
    SV_WI(*a, 0x4c) = 1;
    SV_WI(*a, 0x1c0) = 3;
    id = d2_WinOpenPic(x, y + 0x2a, 4, f);
    *b = id;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x130) + fr) = id;
    SV_WI(*b, 0x4c) = 1;
    SV_WI(*b, 0x1c0) = 3;
    SV_WI(*b, 0x1e0) = g;
    SV_WI(*a, 0x1e0) = g;
    id = d2_CreateWinText(x - 0x13, y + 0x19, ch_EmptyName, f);
    *c = id;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x134) + fr) = id;
    SV_WS(*c, 0x74) = 0x54;
    SV_WI(*c, 0x5c) = 1;
    SV_WS(*c, 0x14) = 0x30;
    SV_I(((unsigned)&ai_Unkf8f13c + 0x22c) + g * 0x3c4) = 0;
    SV_I(((unsigned)&ai_Unkf8f13c + 0x248) + g * 0x3c4) = p5;
    return 1;
}
