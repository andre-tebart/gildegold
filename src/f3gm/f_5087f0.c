/* 0x5087f0, the price of the object item for n pieces: the prototype's base value (4.0 without a building, else the building's two values) times the amount per unit (+0x22 / +0x36), the sub-objects (four words at +0x2e, weights at +0x26) added; cached in the prototype word +0x38 (value / 32). ROUGH */

float gm_ItemPrice(int item, unsigned char n)
{
    char *p;
    float s, f;
    int i;
    unsigned short *w;
    char buf[0x100];
    unsigned char *bp;

    p = OPROTO(item);
    if (*(int *)(p + 0x38) == 0) {
        if (ai_Unk919f80[item] == 0)
            f = 4.0f;
        else {
            bp = (unsigned char *)(ai_BTypes + ai_Unk919f80[item] * 0x24d);
            f = 0.0f;
            i = 0;
            do {
                f += bp[0x233 + i] * 896.0f * 0.0013888888888888887;
                i++;
            } while (i < 2);
        }
        s = (float)(f * 0.5 * ((double)*(unsigned *)(p + 0x22) / ((*(unsigned short *)(p + 0x36) == 0) + *(unsigned short *)(p + 0x36))));
        if (*p == 0x17)
            return s * 2.2f;
        i = 0;
        w = (unsigned short *)(p + 0x2e);
        do {
            if (*w == 0)
                break;
            if (*w == 0xffff)
                sprintf(buf, "eval_ObjektPreis2Credits:Unterobjekt f\xfcr '%s' nicht vorhanden!!!", (char *)ai_Unk143c860[item][0]);
            else
                s = (float)(gm_ItemPrice(*w, n) * w[-4] / *(unsigned short *)(p + 0x36) + s);
            i++;
            w++;
        } while (i < 4);
        *(int *)(p + 0x38) = (int)(s * 0.06875f);
        return n * s * 0.01f * 2.2f;
    }
    f = (*(int *)(p + 0x38) << 5) * n * 0.01f;
    if (p[0x21] == 3)
        f *= 1.5f;
    return f;
}
