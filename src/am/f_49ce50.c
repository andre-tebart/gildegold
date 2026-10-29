/* 0x49ce50, decide callback 0x28: get a house (kind 5) or the good 0x15 for it, else what 0x4aa6a0 finds,
   if the other character (0x504e20) is willing */

int ai_Unk49ce50(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned char *h;
    AIARG s1;
    AIARG s2;
    float g;
    int r;

    if (f != 0)
        return 0;
    h = *(unsigned char **)((char *)p + 0x170);
    if (h == 0) {
        if (((unsigned char *)p)[2] != 3) {
            s1.unk08 = 1;
            s1.kind = 5;
            s1.unk04 = 4;
            s2.kind = 0;
            s2.unk04 = 0;
            s2.unk08 = 0;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
    } else if (((unsigned char *)p)[2] != 3) {
        if (gm_Query(*(int *)(h + 0x5d), 2, 6, 0, 0x15) == 0) {
            s1.kind = 2;
            s1.unk04 = 0x15;
            s1.unk08 = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.unk04 = *(int *)(h + 1);
            s2.unk08 = 1;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
        goto find;
    }
    if (!(((unsigned char *)p)[0x1e8] & 4))
        return 0;
find:
    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    if (ai_Unk4aa6a0(p, &s1, &s2) == 0)
        return 0;
    g = gm_Mix((AIPER *)p, (AIPER *)&ai_Chars[s2.unk10], 4);
    if (util_RandomF() + g < 1.0f) {
        s1.kind = 4;
        s1.unk04 = 3;
        s1.unk08 = 1;
        s1.unk14 = 0.0f;
        s2.kind = 0;
        s2.unk04 = 0;
        s2.unk08 = 0;
        r = ai_Unk495a50(0x28, p->id, 0, &s1, &s2);
        goto done;
    }
    *o1 = s1;
    *o2 = s2;
    return 0x28;
done:
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
