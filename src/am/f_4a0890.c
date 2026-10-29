/* 0x4a0890, decide callback 0x38 (0x4e95a0 must allow it): by the character's office 0x169: 0x1e someone
   (0x4a0240, kind 7), 0x20 / 0x1f / 0x21 a quarter (0x4a0410 / 0x4a05e0, kind 0x16) */

int ai_Unk4a0890(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c;
    unsigned char k;
    int n;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (amt_CanUseMayorPower3(p) != 1)
        return 0;
    k = ((unsigned char *)p)[0x169];
    if (k == 0x1e) {
        c = ai_Unk4a0240(p);
        if (c == 0)
            return 0;
        o1->kind = 7;
        o1->unk04 = c->netId;
        o1->unk10 = 1;
        return 0x38;
    }
    if (k == 0x20)
        n = ai_Unk4a0410(p);
    else if (k == 0x1f || k == 0x21)
        n = ai_Unk4a05e0(p);
    else
        return 0;
    if (n == 0)
        return 0;
    o1->unk04 = n;
    o1->kind = 0x16;
    o1->unk10 = 4;
    return 0x38;
}
