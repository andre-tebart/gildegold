/* 0x52e6d0, builds the picture frames of the dialog form f (0x4731d0 (x, y, picture, form)): the title bar 0x520 / 0x427 + the building kind, the corners; the picture b = 0x73b selects the
   wide variant; sets the four borders of the form (+0x1fc .. 0x20c) to 0x18 */

void sv_Unk52e6d0(int f, int b, int c)
{
    unsigned char *o;
    int pic, x, y;

    d2_WinOpenPic(599, 9, 0x520, f);
    if (c == 0) {
        o = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding);
        if (o == 0 && (o = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding)) == 0)
            c = 0x520;
        else
            c = *o + 0x427;
    }
    d2_WinOpenPic(0xb, 9, c, f);
    o = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding);
    if (o == 0 && (o = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding)) == 0) {
        pic = 0x444;
        goto l748;
    }
    if (b != 0x73b) {
        pic = *o + 0x427;
l748:
        d2_WinOpenPic(599, 9, pic, f);
        if (b != 0x73b) {
            d2_WinOpenPic(0, 0, b, f);
            d2_WinOpenPic(0, 400, b + 1, f);
            d2_WinOpenPic(0, 0x4d, b + 2, f);
            b = b + 3;
            y = 0x4d;
            x = 0x247;
            goto l815;
        }
    } else {
        d2_WinOpenPic(0x1eb, 9, 0x520, f);
        d2_WinOpenPic(0x1eb, 9, *o + 0x427, f);
    }
    d2_WinOpenPic(0, 0, 0x73b, f);
    d2_WinOpenPic(0, 0x43, 0x73d, f);
    d2_WinOpenPic(0x1f7, 0x43, 0x73e, f);
    b = 0x73c;
    y = 0x1a8;
    x = 0;
l815:
    d2_WinOpenPic(x, y, b, f);
    pic = SV_I((unsigned)&d2_LastWindow) * 0x3c4;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x11c) + pic) = 0x18;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x120) + pic) = 0x18;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x124) + pic) = 0x18;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x128) + pic) = 0x18;
}
