/* 0x49d6a0, decide callback 0x2c: what 0x4aa120 finds for the kind 0x10 / 4 */

int ai_Unk49d6a0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;

    if (f == 0) {
        memset(&s1, 0, sizeof(s1));
        memset(&s2, 0, sizeof(s2));
        s1.unk08 = 0;
        s1.kind = 0x10;
        s1.unk04 = 4;
        if (ai_Unk4aa120(a, p, 0, &s1, &s2) != 0) {
            *o1 = s1;
            *o2 = s2;
            return 0x2c;
        }
    }
    return 0;
}
