/* 0x47dab0, lets the 13 needs of a character grow (value = value + a - b * value, kept in 0..1000) and
   switches its current need to the strongest one */

void ai_GrowNeeds(AICHAR *c)
{
    float *p;
    union {
        float f;
        int i;
    } best;
    int i, bi;

    c->mood[13][2] = min(max(0.0, (c->mood[13][0] + c->mood[13][2]) - c->mood[13][1] * c->mood[13][2]), 1000.0);
    best.f = 0.0;
    bi = best.i;
    for (i = 0; i < 13; i++) {
        if (i != c->sel) {
            if (i == 1)
                c->mood[1][2] = 0;
            else if (i == 0) {
                if (c->money <= 6)
                    goto grow;
                c->mood[0][2] = 0;
            } else if (i == 2) {
                if (c->bld2 == 0 && c->kind != 3)
                    goto grow;
                c->mood[2][2] = 0;
            } else if (i == 3) {
                c->mood[3][2] = min(max(0.0, ((c->mood[3][0]) * c->mood3Rate + c->mood[3][2]) - c->mood[3][1] * c->mood[3][2]), 1000.0);
            } else {
grow:
                p = &c->mood[i][2];
                *p = min(max(0.0, (c->mood[i][0] + *p) - c->mood[i][1] * *p), 1000.0);
            }
        }
        if (i == c->sel) {
            if ((2.0 - c->mood[13][2] * 0.002) * c->mood[i][2] >= best.f) {
                bi = i;
                best.f = (2.0 - c->mood[13][2] * 0.002) * c->mood[i][2];
            }
        } else if (c->mood[i][2] >= best.f) {
            best.f = c->mood[i][2];
            bi = i;
        }
    }
    if (bi != c->sel) {
        p = &c->mood[c->sel][2];
        if (*p >= 1000.0f)
            *p = *p - 50.0f;
        c->sel = (unsigned char)bi;
        c->mood[13][2] = 0;
    }
}
