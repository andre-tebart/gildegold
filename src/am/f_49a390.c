/* 0x49a390, decide callback: a building of type 0x161 for the character's workshop, if its workers are
   not too tired */

int ai_Unk49a390(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k;
    int b;
    float x;
    int st[3];
    int rec[4];
    AIARG s1;
    AIARG s2;
    int o;
    int n, sum, r;
    unsigned char *c;

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
    if (ai_Unk4a63f0((AIPER *)p, 4, 0x90010, 0, 100.0f, &b) == 0)
        return 0;
    G8_4FEA50(&o, 0, 0, b);
    if (o == 0)
        return 0;
    n = 0;
    sum = 0;
    for (c = (unsigned char *)ai_CharsT; c < (unsigned char *)(ai_CharsT + 768); c += 0x21c) {
        if (*(int *)(c + 0x16c) == o) {
            n++;
            sum += c[0x1c5];
        }
    }
    x = (float)util_Random(4);
    if (x + (float)sum / (float)n > 6.0f)
        return 0;
    rec[1] = 0;
    rec[2] = 0;
    rec[3] = 0;
    rec[0] = 0x161;
    ai_Unk4a5760(p, 1, rec, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 1)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            s1.unk04 = rec[0];
            s2.unk04 = p->netId;
            s1.kind = 2;
            s1.unk08 = 1;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.unk08 = 1;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return 0;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    s1.kind = 1;
    s1.unk04 = rec[2];
    s1.unk08 = 1;
    *o1 = s1;
    s2.kind = 4;
    s2.unk04 = *(int *)((char *)o + 1);
    s2.unk08 = -1;
    s2.unk10 = 0x1b;
    *o2 = s2;
    return 0x1b;
}
