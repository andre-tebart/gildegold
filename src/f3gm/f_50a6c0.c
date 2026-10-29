/* 0x50a6c0, true when the character p (a person of the kinds 6 / 7 or another) is too far from its goal after n more: the pseudocode lost the float operands (0x509e60, 0x50a640, the two __ftol); the person of the kinds 6 / 7 is reported to its owner through 0x5b92e0 / 0x53dda0 (text 0x42c3 = ...) when the ratio is above 0.07 (above 0.2 for the others). ROUGH */

int gm_Unk50a6c0(unsigned short *p, int n)
{
    int a, b;
    char msg[2048];
    float ratio;
    int r;

    a = gm_GetGoodsValue(p);
    if (0.0f < (float)(a + n))
        return 0;
    gm_GetWorth(*p);
    if (*((char *)p + 2) == 6 || *((char *)p + 2) == 7) {
        a = (int)gm_GetGoodsValue(p);                /* lost: float operand of the first __ftol */
        b = (int)gm_GetGoodsValue(p);                /* lost */
        ratio = (float)(b < 0 ? -b : b) / (float)a;
        if (0.07f < ratio) {
            txt_Format(msg, 0x42c3);
            r = ai_Unk5b92e0(*(int *)(p + 2), p, a, msg);
            he_ShowNews(*(int *)(p + 2), *(int *)(p + 2), msg, 1, 0x5ff, "_NACHRICHTEN_HS_69");
            return r <= a / 2;
        }
    } else {
        b = (int)gm_GetGoodsValue(p);                /* lost */
        ratio = (float)(b < 0 ? -b : b) / (float)a;
        if (0.2f < ratio)
            return 1;
    }
    return 0;
}
