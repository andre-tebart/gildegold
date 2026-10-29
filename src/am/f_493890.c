/* 0x493890, what side c does next in the duel (0 none, 1 ... 4) */

int ai_Unk493890(AGDUEL *d, AGCH *c)
{
    unsigned char b1, b2;
    unsigned char *t;
    unsigned int u;
    int me, n, i, sum, s;
    float f, g;

    if (c->kind == 10) {
        if (c == d->side1) {
            s = d->kind1;
            u = d->unk14;
            b1 = d->n1;
            t = d->t1;
            b2 = d->n2;
            me = 0;
        } else {
            if (c != d->side2)
                return 0;
            s = d->kind2;
            u = d->unk38;
            b1 = d->n2;
            t = d->t2;
            b2 = d->n1;
            me = 1;
        }
        n = b1;
        i = 0;
        sum = 0;
        if (n > 0) {
            do {
                sum += t[i];
                i++;
            } while (i < n);
        }
        switch (s) {
        case 0:
            return 1;
        case 1:
        case 2:
            sum = 0x11 - sum;
            if (sum > 5)
                return 2;
            if (sum == 0)
                return 3;
            f = ((1.0f - (u & 0xff) * 0.2f) * 0.25f + 1.0f) * (sum * 0.16666667f);
            if (util_RandomF() < f)
                return 2;
            break;
        case 3:
            f = (sum + me) * 0.05882353f;
            if (1.0f <= f)
                return 3;
            g = (float)(int)(6 - (u & 0xff));
            f = f - b2 * 3.5f;
            if (f > g)
                return 4;
            break;
        default:
            return 0;
        }
        return 3;
    }
    return 0;
}
