/* 0x49ec10, eval callback 0x32: rated 6 / 2 times higher when 0x5044b0(p, 4) has more than two entries and
   0x4aaef0 finds something; else only from hour 20 on */

int ai_Unk49ec10(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int q;

    *a = -1e30f;
    *b = -1e30f;
    if (f != 0)
        return 0;
    q = gm_Unk5044b0(p->id, 4);
    if (q != 0 && gm_TitlesLeft(q) > 2) {
        if (ai_Unk4aaef0(p, &s1, &s2) == 0)
            return 0;
        G8_4A4AE0(a, b, i, p, 0, o1, o2);
        *o1 = s1;
        *o2 = s2;
        *a *= 6.0f;
        *b *= 2.0f;
        return 1;
    }
    if (ai_Time.hour < 0x14)
        return 0;
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
}
