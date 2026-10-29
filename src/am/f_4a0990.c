/* 0x4a0990, eval callback 0x38: by the office 0x169 (0x1e..0x21) the office building (kind 0x17..0x1a); rated
   three times higher while its level 0x5c is below 50 + a random 0..15 */

int ai_Unk4a0990(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *bl;
    unsigned char k;

    *a = -100.0f;
    *b = -100.0f;
    k = ((unsigned char *)p)[0x169];
    if (k < 0x1e && k > 0x21)
        return 0;
    if (k == 0x1e)
        bl = gm_BuildQuery(1, 5, 0x17);
    else if (k == 0x1f)
        bl = gm_BuildQuery(1, 5, 0x18);
    else if (k == 0x20)
        bl = gm_BuildQuery(1, 5, 0x19);
    else
        bl = gm_BuildQuery(1, 5, 0x1a);
    if (bl == 0)
        return 0;
    if (bl->level < util_Random(0x10) + 0x32) {
        G8_4A4AE0(a, b, i, p, f, o1, o2);
        *a *= 3.0f;
        *b *= 3.0f;
        return 1;
    }
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
}
