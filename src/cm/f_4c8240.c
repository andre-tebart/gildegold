/* 0x4c8240, handler of type 0x1b: changes the relations (0x7e5360 / 0x889f80, 0x300 x 0x300) between the characters
   data[0] and data[4]; kind data[0xc]: 4 drifts all relations by level data[0x14], 3 scales column b by data[0x10],
   0..2 add data[8] */

int ai_Unk4c8240(G17MSG *m, G17STAT *s)
{
    int a, b, kind, i, j, v, hi, lo, dn, dp;
    unsigned int n;

    a = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    *(int *)&m->data[0] = a;
    b = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
    kind = *(int *)&m->data[0xc];
    *(int *)&m->data[4] = b;
    if (kind == 4) {
        n = *(unsigned int *)&m->data[0x14];
        if (n != 0 && n >= 4) {
            hi = 0x28;
            lo = -0x73;
            dn = -0x14;
            dp = 3;
        } else if (n == 3) {
            hi = 0x37;
            lo = -0x69;
            dn = -0x10;
            dp = 5;
        } else if (n == 2) {
            hi = 0x46;
            lo = -0x5f;
            dn = -0xc;
            dp = 7;
        } else if (n == 1) {
            hi = 0x55;
            lo = -0x55;
            dn = -8;
            dp = 9;
        } else {
            hi = 100;
            lo = -0x4b;
            dn = -5;
            dp = 0xb;
        }
        for (i = 0; i < 0x300; i++) {
            for (j = 0; j < 0x300; j++) {
                v = ai_Relations[i][j];
                if (i != j && *(unsigned short *)ai_Chars[i] != 0xffff && *(unsigned short *)ai_Chars[j] != 0xffff) {
                    if (v > hi)
                        v += dn;
                    else if (v < lo)
                        v += dp;
                    if (v <= -0x7f)
                        v = -0x7f;
                    else if (v >= 0x7f)
                        v = 0x7f;
                    ai_Relations[i][j] = (char)v;
                }
            }
        }
    } else {
        for (i = 0; i < 0x300; i++)
            if (*(int *)(ai_Chars[i] + 4) == b)
                break;
        if (i >= 0x300)
            return 1;
        if (*(unsigned short *)ai_Chars[i] == 0xffff)
            return 1;
        if (kind == 3) {
            v = *(int *)(ai_Chars[i] + 0x210);
            for (j = 0; j < 0x300; j++)
                if (*(int *)(ai_Chars[j] + 4) != v)
                    ai_RelationMemory[j][i] = (char)(int)(ai_RelationMemory[j][i] * *(float *)&m->data[0x10]);
        } else {
            for (j = 0; j < 0x300; j++)
                if (*(int *)(ai_Chars[j] + 4) == a)
                    break;
            if (j >= 0x300)
                return 1;
            if (*(unsigned short *)ai_Chars[j] == 0xffff)
                return 1;
            if (kind == 2) {
                v = ai_Relations[j][i];
                ai_RelationMemory[j][i] = 0;
                v += *(int *)&m->data[8];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[j][i] = (char)v;
            } else if (kind == 1) {
                v = ai_Relations[j][i] + *(int *)&m->data[8];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[j][i] = (char)v;
                if (*(int *)&m->data[8] >= 0)
                    v /= 2;
                v += ai_RelationMemory[j][i];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_RelationMemory[j][i] = (char)v;
            } else if (kind == 0) {
                v = ai_Relations[j][i] + *(int *)&m->data[8];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[j][i] = (char)v;
            }
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
