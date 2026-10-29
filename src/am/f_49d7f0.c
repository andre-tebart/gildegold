/* 0x49d7f0, eval callback (characters with +0x166 == 0xf): rated three times higher when 0x4f9e00 is above
   the character's value +0x100 */

int ai_Unk49d7f0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    float d[10];

    if (((unsigned char *)p)[0x166] != 0xf) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    unk_Unk4f9e00(d);
    if ((*(float *)((char *)p + 0x100) - 2.0) * 0.4f < d[3]) {
        G8_4A4AE0(a, b, i, p, f, o1, o2);
        *a *= 3.0f;
        *b *= 3.0f;
        return 1;
    }
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
}
