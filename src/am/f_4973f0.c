/* 0x4973f0, evaluate: the default rating, ten times for a character with 0x167 set and 0x166 clear */

int ai_Unk4973f0(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    float k;

    if (f) {
        *a = -1.0e30f;
        *b = -1.0e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    if (p->unk167 && !p->unk166)
        k = 10.0f;
    else
        k = 1.0f;
    ai_Unk4a49a0(a, b, i, p, 0, o1, o2);
    *a *= k;
    *b *= k;
    return 1;
}
