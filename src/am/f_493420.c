/* 0x493420, one side gets a die (k == 0) or a roll of dice (k != 0) */

void ai_Unk493420(AGDUEL *d, AGCH *c, int k)
{
    unsigned char *t, *n, lvl;
    float f, r;
    int i, j, sum, w, best, a, b, g, extra;
    short h, sv;

    if (c == d->side1) {
        t = d->t1;
        n = &d->n1;
    } else {
        t = (unsigned char *)c;
        n = (unsigned char *)c;
        if (c == d->side2) {
            t = d->t2;
            n = &d->n2;
        }
    }
    f = gm_GetSkill(c, 2);
    lvl = (int)(f * 100.0f) / 20;
    if (lvl >= 4)
        lvl = 4;
    if (k != 0) {
        h = util_Random(3);
        *n = 3;
        if (f > 0.8888f) {
            sv = util_Random(0x10);
            extra = 3 - (sv != 0);
        } else if (f > 0.5555f) {
            sv = util_Random(0x10);
            extra = 2 - (sv != 0);
        } else {
            sv = util_Random(f > 0.3333f ? 4 : 8);
            extra = sv == 0;
        }
        r = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (r <= ag_Chance[lvl][i]) {
                a = i + 1;
                break;
            }
        }
        r = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (r <= ag_Chance[lvl][i]) {
                b = i + 1;
                break;
            }
        }
        r = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (r <= ag_Chance[lvl][i]) {
                g = i + 1;
                break;
            }
        }
        t[1] = (unsigned char)b;
        t[0] = (unsigned char)a;
        t[2] = (unsigned char)g;
        if (a == b && a == g)
            return;
        if (extra == 0)
            return;
        if (h == 0) {
            for (i = 0; i < 3; i++) {
                if (extra == 0)
                    return;
                w = t[i] + 1;
                if (w > 6)
                    w = 6;
                t[i] = (unsigned char)w;
                extra--;
            }
            return;
        }
        if (h != 1) {
            j = util_Random(3);
            w = t[j] + extra;
            if (w > 6)
                w = 6;
            t[j] = (unsigned char)w;
            return;
        }
        best = -1;
        lvl = 6;
        for (i = 0; i < 3; i++) {
            if (t[i] < lvl) {
                best = i;
                lvl = t[i];
            }
        }
        if (best < 0)
            return;
        w = t[best] + extra;
        if (w > 6)
            w = 6;
        t[best] = (unsigned char)w;
        return;
    }
    f = gm_GetSkill(c, 0);
    r = util_RandomF();
    sum = 0;
    for (i = 0; i < *n; i++)
        sum += t[i];
    if (f <= 0.66) {
        if (f <= 0.44) {
            if (sum <= 0xe)
                goto roll6;
            best = -1;
        } else if (sum <= 0xd) {
            goto roll5;
        } else
            best = -1;
    } else {
        if (sum <= 0xc)
            goto roll4;
        best = -1;
    }
    goto pick;
roll4:
    if (sum < 6)
        best = 1;
    else
        best = util_Random(3) - 1;
    goto pick;
roll5:
    if (sum < 5)
        best = 1;
    else
        best = util_Random(3) - 1;
    goto pick;
roll6:
    if (sum < 4)
        best = 1;
    else
        best = util_Random(3) - 1;
pick:
    for (j = 0; j < 6; j++) {
        if (r <= ag_Chance[lvl][j])
            break;
    }
    if (j >= 6) {
        t[*n] = (unsigned char)w;
        (*n)++;
        return;
    }
    w = best + 1 + j;
    if (w >= 6)
        w = 6;
    else if (w < 2)
        w = 1;
    t[*n] = (unsigned char)w;
    (*n)++;
}
