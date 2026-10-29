/* 0x4cee20, shows / moves the floating numbers; one that rose for 128 steps is removed. Rough draft. */

void ai_Unk4cee20()
{
    char buf[0x40];
    int box[4];
    int i, x, y0;
    G18POP *p;

    for (i = 0, p = ai_Unk9304c0; (int)p < (int)(ai_Unk9304c0 + 16); i++, p++) {
        if (p->v == 0)
            continue;
        if (d3if_ScreenBox(*(int *)(*(char **)(p->c + 0x184) + 0x34), box) == 0) {
            if (ai_Unk9304c0[i].h != -1)
                d2_EnableObject(ai_Unk9304c0[i].h, 0);
            return;
        }
        x = box[0] + 0x10;
        y0 = box[1] - 0x30;
        if (p->h == -1) {
            if (p->kind == 1)
                sprintf(buf, "%c-%i%c", 0x5c, p->v, 0x5d);
            else if (p->kind == 2)
                sprintf(buf, "%c-%i%c", 0x5e, p->v, 0x5f);
            else
                sprintf(buf, "-%i", p->v);
            p->h = d2_CreateScreenText(x, (int)(y0 + p->t), buf);
        } else
            d2_MoveObj(x, (int)(y0 + p->t), p->h);
        if (p->h != -1)
            d2_EnableObject(p->h, 1);
        p->t = (float)(p->t - 0.5);
        if (p->t <= 0.0f) {
            d2_RemoveObj(p->h);
            p->v = 0;
            p->h = -1;
            p->c = 0;
        }
    }
}
