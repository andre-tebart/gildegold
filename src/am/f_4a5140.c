/* 0x4a5140: may character p build the building type s1->unk04 (s1 kind 5)? A type of class 2 must rank above
   the one it has (0x170) and pass 0x4f81c0 (2 or 4) for types 4..7; other types ask 0x5020e0. With f, s2 becomes
   the plot (kind 0xc) that 0x4a4e70 finds */

int ai_Unk4a5140(int a, AIPER *p, int b, AIARG *s1, AIARG *s2, int f)
{
    int q[0x88];
    unsigned char *h;
    int k, r;

    memset(q, 0, sizeof(q));
    q[0] = 1;
    if (s1->kind != 5)
        return 0;
    k = s1->unk04;
    if (ai_BTypesT[k].kind == 2) {
        h = *(unsigned char **)((unsigned char *)p + 0x170);
        if (h != 0 && ai_BTypesT[k].cap247 <= ai_BTypesT[*h].cap247 && ai_BTypesT[*h].kind == 2)
            return 0;
        if (k == 4) {
            r = ord_DispatchOrder(1, p, q);
            if (r != 2 && r != 4)
                return 0;
        } else if (k == 5) {
            r = ord_DispatchOrder(5, p, q);
            if (r != 2 && r != 4)
                return 0;
        } else if (k == 6) {
            r = ord_DispatchOrder(9, p, q);
            if (r != 2 && r != 4)
                return 0;
        } else if (k == 7) {
            r = ord_DispatchOrder(0xc, p, q);
            if (r != 2 && r != 4)
                return 0;
        }
    } else if (unk_Unk5020e0(k, *(unsigned short *)p) == 0) {
        return 0;
    }
    if (f == 0)
        return 1;
    r = ai_Unk4a4e70(p, s1->unk04);
    if (r == 0)
        return 0;
    s2->unk04 = r;
    s2->kind = 0xc;
    s2->unk08 = 1;
    return 1;
}
