/* 0x5086b0, the value of the object n: the base values of its building (two bytes at +0x233) times its amount per unit. ROUGH */

float gm_ObjectValue(int n)
{
    float f;
    int i;
    char *p;

    f = 0.0f;
    i = 0;
    do {
        f += (unsigned char)(ai_BTypes + ai_Unk919f80[n] * 0x24d)[0x233 + i] * 896.0f * 0.0013888888888888887;
        i++;
    } while (i < 2);
    p = OPROTO(n);
    return f * *(int *)(p + 0x22) / *(unsigned short *)(p + 0x36);
}
