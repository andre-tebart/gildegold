/* 0x49ed10, decide callback 0x32: what 0x4aaef0 finds (when 0x5044b0(p, 4) has more than two entries),
   else what 0x4ab080 finds */

int ai_Unk49ed10(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int q;

    q = gm_Unk5044b0(p->id, 4);
    if (a != 0x32 && a != 0x25 && a != 0)
        return 0;
    if (f != 0)
        return 0;
    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    if (q != 0 && gm_TitlesLeft(q) > 2) {
        if (o1->kind == 4 && o2->kind == 1)
            return 0x32;
        if (ai_Unk4aaef0(p, &s1, &s2) != 0) {
            *o1 = s1;
            *o2 = s2;
            return 0x32;
        }
    }
    if (ai_Unk4ab080(p, &s1, &s2) != 0) {
        *o1 = s1;
        *o2 = s2;
        return 0x32;
    }
    return 0;
}
