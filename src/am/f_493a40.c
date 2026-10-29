/* 0x493a40, the next phase of the duel */

void ai_Unk493a40(AGDUEL *d)
{
    int ph, i, n, sum1, sum2, s1, s2;

    ph = d->phase;
    i = 0;
    s1 = 0;
    s2 = 0;
    sum2 = 0;
    sum1 = 0;
    if (ph == 1) {
        d->phase = 2;
        return;
    }
    if (ph == 2) {
        if (d->t1[0] == d->t1[1] && d->t1[0] == d->t1[2])
            s1 = d->t1[2] + d->t1[1] + d->t1[0];
        if (d->t2[0] == d->t2[1] && d->t2[0] == d->t2[2])
            s2 = d->t2[2] + d->t2[1] + d->t2[0];
        if (s1 <= s2) {
            if (s2 > 0) {
                d->phase = 6;
                return;
            }
            if (d->kind1 == 4) {
                d->phase = 6;
                return;
            }
            d->phase = (d->kind2 == 4) * 2 + 3;
            return;
        }
    } else {
        n = d->n1;
        if (n > 0) {
            do {
                sum1 += d->t1[i];
                i++;
            } while (i < n);
        }
        i = 0;
        n = d->n2;
        if (n > 0) {
            do {
                sum2 += d->t2[i];
                i++;
            } while (i < n);
        }
        if (ph == 3) {
            if (sum1 < 0x12 && d->kind1 != 4) {
                if (d->kind1 == 3 && d->kind2 == 3) {
                    d->phase = (sum1 <= sum2) + 5;
                    return;
                }
                d->phase = 4;
                return;
            }
            d->phase = 6;
            return;
        }
        if (ph != 4)
            return;
        if (sum2 < 0x12 && d->kind2 != 4) {
            if (d->kind1 == 3 && d->kind2 == 3) {
                d->phase = (sum1 <= sum2) + 5;
                return;
            }
            d->phase = 3;
            return;
        }
    }
    d->phase = 5;
}
