/* 0x508740, the wage of a job (a) or title (b) of the building kind k for the character c, reduced for the kinds 6 / 7 */

float gm_Unk508740(char *c, int k, int a, int b)
{
    float f;
    char *p;

    f = 0.0f;
    p = ai_BTypes + k * 0x24d;
    if (a == -1) {
        if (b != -1)
            f = (unsigned char)p[0x233 + b] * 0x380;
    } else
        f = (unsigned char)p[0x229 + a] * 0x380;
    if (c[2] == 6 || c[2] == 7)
        f = (1.0 - (2 - ai_Difficulty) * 0.25) * f;
    return f;
}
