/* 0x47cb90, steps all animations */

void d2_StepAnims(void)
{
    int i, ok;
    int step;
    char back;
    D2ANIM *a;
    D2BANK *b;
    unsigned char *t;

    ok = 1;
    if (tb_AnimTick - d2_LastAnimTick != 0) {
        if (tb_AnimTick - d2_LastAnimTick < 1)
            step = 1;
        else
            step = tb_AnimTick - d2_LastAnimTick;
        d2_LastAnimTick = tb_AnimTick;
        for (i = 0; i < 16; i++) {
            a = &d2_Anims[i];
            b = a->bank;
            if (b != 0 && a->frame >= 0) {
                a->timer = a->timer + step;
                t = (unsigned char *)b + b->pal;
                if ((unsigned char)a->timer > t[a->frame * 8 + 1]) {
                    back = a->back;
                    if (back == 0)
                        a->frame++;
                    if (a->back == 1)
                        a->frame--;
                    a->timer = 0;
                    if (a->frame >= b->npal && back == 0) {
                        if (b->pad42 == 0) {
                            a->frame = 0;
                            a->shape = t[0];
                        }
                        if (b->pad42 == 1) {
                            a->frame = -1;
                            a->shape = t[b->npal * 8 - 8];
                        }
                        if (b->pad42 == 3) {
                            a->frame = 0;
                            a->shape = t[0];
                            d2_FreeAnim(i);
                        }
                        ok = 0;
                    }
                    if (a->frame < 0 && a->back == 1) {
                        if (b->pad42 == 0) {
                            a->frame = b->npal - 2;
                            a->shape = t[a->frame * 8];
                        }
                        if (b->pad42 == 1) {
                            a->frame = -1;
                            a->shape = t[0];
                        }
                        if (b->pad42 == 3) {
                            a->frame = 0;
                            a->shape = t[0];
                            d2_FreeAnim(i);
                        }
                        ok = 0;
                    } else if (ok) {
                        a->shape = t[a->frame * 8];
                    }
                }
            }
        }
    }
}
