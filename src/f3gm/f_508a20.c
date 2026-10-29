/* 0x508a20, a health factor of the character c: 1 + hp / 1008 above 42 */

float gm_HealthFactor(int c)
{
    float f;
    unsigned char *p;

    p = (unsigned char *)ai_Chars + c * 0x21c;
    if (p == 0)
        return 1.0f;
    f = p[0x80];
    if (f <= 42.0f)
        return 1.0f;
    return f * 0.000992063492063492 + 1.0;
}
