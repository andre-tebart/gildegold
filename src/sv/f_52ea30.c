/* 0x52ea30, resizes the form f to the size of its background picture (0x6ed918 + index * 0x54, +0x50 / +0x52) when it has one (0xf8f4a8 != -1) and sets its borders 0x18 / 0x18 / 0x1b / 0xb */

void sv_FitForm(int f)
{
    int p;

    f = f * 0x3c4;
    if (SV_I(((unsigned)&ai_Unkf8f39c + 0x10c) + f) != -1) {
        p = SV_I((unsigned)&d2_Pics) + SV_I(((unsigned)&ai_Unkf8f39c + 0x10c) + f) * 0x54;
        SV_S((unsigned)&ai_Unkf8f128 + f) = *(short *)(p + 0x50);
        SV_S(((unsigned)&ai_Unkf8f128 + 0x2) + f) = *(short *)(p + 0x52);
        SV_I(((unsigned)&ai_Unkf8f39c + 0x11c) + f) = 0x18;
        SV_I(((unsigned)&ai_Unkf8f39c + 0x120) + f) = 0x18;
        SV_I(((unsigned)&ai_Unkf8f39c + 0x124) + f) = 0x1b;
        SV_I(((unsigned)&ai_Unkf8f39c + 0x128) + f) = 0xb;
    }
}
