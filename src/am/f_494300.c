/* 0x494300, rolls the plan's tries: sets unk48 on success, unk4c to the negated sum */

int ai_Unk494300(AIPLAN *q)
{
    AIREC10 *t;
    float f, lim, scale, acc, v, sum;
    int i;

    q->unk48 = 0;
    t = ai_Unk4948c0(q->unk38);
    f = q->unk0c;
    lim = f * 0.33f;
    i = 0;
    acc = 0.0f;
    for (; i < q->unk3c; i++) {
        scale = 0.5f;
        if (q->unk40 == 0)
            scale = 1.0f;
        v = ai_Unk4e5360() * t->unk0c * scale + t->unk08;
        sum = acc + v;
        if (sum >= f) {
            acc = v + acc;
            if (ai_Unk4e52f0(4))
                acc = acc * 0.75;
            q->unk48 = 1;
            break;
        }
        if (v > lim) {
            acc = acc + lim;
            q->unk48 = 1;
            break;
        }
        lim = lim * 0.8f;
        acc = sum;
    }
    q->unk4c = -acc;
    return q->unk48;
}
