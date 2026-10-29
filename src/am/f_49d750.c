/* 0x49d750, decide callback 0x2d: what 0x4aa120 finds */

int ai_Unk49d750(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;

    if (a != 0)
        return 0;
    if (f != 0)
        return 0;
    s1.kind = 0;
    s1.unk04 = 0;
    s1.unk08 = 0;
    if (ai_Unk4aa120(0, p, 0, &s1, &s2) == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return 0x2d;
}
