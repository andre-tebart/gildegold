/* 0x4a8190, the showdown: the winning seat (seat 0 against 1, the better of them against 2), its hand into unkbc */

int ai_Unk4a8190(void)
{
    int g1[3], r1[3], kind[3], out[3], g2[3];
    int r2[3];
    unsigned int i, w;
    int k, a, b;

    kind[0] = 0;
    kind[2] = 0;
    kind[1] = 0;
    out[0] = 0;
    out[2] = 0;
    out[1] = 0;
    for (i = 0; i < 3; i++) {
        if (ai_Unkf83654->seat[i].unk28 < 0) {
            kind[i] = ai_Unk4a7f50(i, &g1[i], &g2[i], &r1[i], &r2[i]);
            ai_Unkf83654->seat[i].unk30 = kind[i];
        } else {
            out[i] = 1;
        }
    }
    if (out[0] != 0) {
        w = 1;
    } else if (out[1] != 0) {
        w = 0;
    } else if (kind[0] == 0) {
        if (kind[1] == 0)
            w = ai_Unk4a76d0(ai_Unkf83654->seat[0].card, 0) <= ai_Unk4a76d0(ai_Unkf83654->seat[1].card, 0);
        else
            w = 1;
    } else if (kind[1] == 0 || kind[1] < kind[0]) {
        w = 0;
    } else if (kind[0] < kind[1]) {
        w = 1;
    } else {
        switch (kind[0]) {
        case 1:
            if (g1[0] > g1[1])
                w = 0;
            else if (g1[0] < g1[1])
                w = 1;
            else
                w = r1[0] <= r1[1];
            break;
        case 2:
            if (g1[1] < g1[0])
                w = 0;
            else if (g1[0] < g1[1])
                w = 1;
            else if (g2[1] < g2[0])
                w = 0;
            else if (g2[1] <= g2[0])
                w = r1[0] <= r1[1];
            else
                w = 1;
            break;
        case 3:
        case 7:
            w = g1[0] <= g1[1];
            break;
        case 4:
            w = ai_Unk4a76d0(ai_Unkf83654->seat[0].card, 0) <= ai_Unk4a76d0(ai_Unkf83654->seat[1].card, 0);
            break;
        case 5:
            r1[0] = g1[0];
            r1[1] = g1[1];
            w = r1[0] <= r1[1];
            break;
        case 6:
            w = r1[0] <= r1[1];
            break;
        case 8:
            w = r1[0] <= r1[1];
            break;
        }
    }
    if (out[2] != 0) {
        ai_Unkf83654->unkbc = kind[w];
        return w;
    }
    k = kind[w];
    if (k == 0) {
        if (kind[2] != 0) {
            ai_Unkf83654->unkbc = kind[2];
            return 2;
        }
        a = ai_Unk4a76d0(ai_Unkf83654->seat[w].card, 0);
        b = ai_Unk4a76d0(ai_Unkf83654->seat[1].card, 0);
        if (b < a)
            return w;
        return 1;
    }
    if (kind[2] != 0 && kind[2] >= k) {
        if (k < kind[2]) {
            ai_Unkf83654->unkbc = kind[2];
            return 2;
        }
        switch (k) {
        case 1:
            if (g1[w] <= g1[2]) {
                if (g1[w] < g1[2] || r1[w] <= r1[2]) {
                    ai_Unkf83654->unkbc = kind[2];
                    return 2;
                }
            }
            break;
        case 2:
            if (g1[w] <= g1[2]) {
                if (g1[w] < g1[2]) {
                    ai_Unkf83654->unkbc = kind[2];
                    return 2;
                }
                if (g2[w] <= g2[2]) {
                    if (g2[w] < g2[2] || r1[w] <= r1[2]) {
                        ai_Unkf83654->unkbc = kind[2];
                        return 2;
                    }
                }
            }
            break;
        case 3:
        case 5:
        case 7:
            if (g1[w] <= g1[2]) {
                ai_Unkf83654->unkbc = kind[2];
                return 2;
            }
            break;
        case 4:
            if (ai_Unk4a76d0(ai_Unkf83654->seat[w].card, 0) <= ai_Unk4a76d0(ai_Unkf83654->seat[2].card, 0))
                w = 2;
            ai_Unkf83654->unkbc = kind[w];
            return w;
        case 6:
        case 8:
            if (r1[w] <= r1[2]) {
                ai_Unkf83654->unkbc = kind[2];
                return 2;
            }
            break;
        default:
            return w;
        }
    }
    ai_Unkf83654->unkbc = k;
    return w;
}
