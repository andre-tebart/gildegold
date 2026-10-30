/* 0x4984d0, decide callback: buy or upgrade a house (called directly by 0x498c70) */

int ai_Unk4984d0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned int up[5];
    AIARG s1;
    AIARG s2;
    unsigned char *b;
    unsigned char *h;
    unsigned int lo32, hi32;
    int money, cost, j;
    unsigned char t;

    up[1] = 7;
    up[2] = 6;
    up[3] = 5;
    up[4] = 4;
    if (((unsigned char *)p)[2] == 3)
        return 0;
    if (f == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (f == 1 && o1->kind != 5 && o1->kind != 4) {
        return 0;
    }
    if (he_FindEvent(2, 0, 0xd, 2, (int)(short)p->id) != 0)
        return 0;
    b = (unsigned char *)ai_BldsT;
    for (up[0] = 0; ; ) {
        if (b[0] != 0) {
            t = ((unsigned char *)ai_BTypesT)[b[0] * 0x24d];
            lo32 = 0;
            hi32 = 0;
            if (t < 0x20)
                lo32 = 1 << t;
            else
                hi32 = 1 << (t - 0x20);
            if (*(short *)(b + 0x27) == (short)p->id && ((lo32 & 0xe07d43b0) != 0 || (hi32 & 1) != 0)) {
                money = (int)G6_509EE0(p, ai_OwnPlayer);
                s2.kind = 0;
                s2.unk04 = 0;
                s2.unk08 = 0;
                if (o1->kind == 0) {
                    h = *(unsigned char **)((char *)p + 0x170);
                    if (h != 0 && ((unsigned char *)ai_BTypesT)[h[0] * 0x24d] == 2 && (h[0x5a] & 0x41) == 0) {
                        t = ((unsigned char *)ai_BTypesT)[h[0] * 0x24d + 0x247];
                        if (t < 2 || t == 3) {
                            s1.kind = 5;
                            j = 0;
                            if (h[0] != 7) {
                                do
                                    j++;
                                while (up[j + 1] != h[0]);
                            }
                            s1.unk04 = up[j];
                            s1.unk08 = 1;
                            s2.kind = 4;
                            s2.unk04 = *(int *)(h + 1);
                            s2.unk08 = 1;
                            if (ai_Unk4a5140(a, p, f, &s1, &s2, 0) == 0)
                                return 0;
                            cost = gm_Unk509dc0(p, s1.unk04);
                            if (cost <= money)
                                goto take;
                            if ((float)(cost - money) / (float)cost >= 0.25f)
                                return 0;
                            o1->unk08 = cost - money;
                            o1->kind = 0x12;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                        if (t == 5)
                            return 0;
                    }
                    s1.kind = 5;
                    s1.unk04 = 6;
                    s1.unk08 = 1;
                    if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) != 0) {
                        cost = gm_Unk509dc0(p, s1.unk04);
                        if (cost <= money)
                            goto take;
                        if ((float)(cost - money) / (float)cost < 0.25f) {
                            o1->unk08 = cost - money;
                            o1->kind = 0x12;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                    }
                    s1.kind = 5;
                    s1.unk04 = 4;
                    s1.unk08 = 1;
                    if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) == 0)
                        return 0;
                    cost = gm_Unk509dc0(p, s1.unk04);
                    if (money < cost) {
                        if ((float)(cost - money) / (float)cost >= 0.25f)
                            return 0;
                        o1->unk08 = cost - money;
                        o1->kind = 0x12;
                        return ai_Unk49eec0(0x11, p, f, o1, o2);
                    }
                } else {
                    if (o1->kind != 5)
                        return 0;
                    h = *(unsigned char **)((char *)p + 0x170);
                    s1 = *o1;
                    if (h == 0) {
                        if (((unsigned char *)ai_BTypesT)[o1->unk04 * 0x24d] != 2)
                            return 0;
                        if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) == 0)
                            return 0;
                        if (((unsigned char *)p)[2] != 5)
                            return 0;
                        cost = gm_Unk509dc0(p, s1.unk04);
                        if (money < cost) {
                            if ((float)(cost - money) / (float)cost >= 0.25f)
                                return 0;
                            o1->kind = 0x12;
                            o1->unk08 = cost - money;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                    } else {
                        t = h[0];
                        if (((unsigned char *)ai_BTypesT)[t * 0x24d] != 2)
                            return 0;
                        s1.unk04 = o1->unk04;
                        if (s1.unk04 - t != 1)
                            return 0;
                        if (t == 5)
                            return 0;
                        if (t == 7)
                            return 0;
                        s1.unk08 = 1;
                        s2.kind = 4;
                        s2.unk04 = *(int *)(h + 1);
                        s2.unk08 = 1;
                        cost = (int)gm_BuildingKindValue(s1.unk04);
                        if (ai_Unk4a5140(a, p, f, &s1, &s2, 0) == 0)
                            return 0;
                        if (money < cost) {
                            if ((float)(cost - money) / (float)cost >= 0.25f)
                                return 0;
                            o1->kind = 0x12;
                            o1->unk08 = cost - money;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                    }
                }
take:
                *o1 = s1;
                *o2 = s2;
                return 0x11;
            }
        }
        up[0]++;
        b += 0xa9;
        if ((int)up[0] > 0xff)
            return 0;
    }
}
