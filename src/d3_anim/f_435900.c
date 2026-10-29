/* 0x435900, the running slot with the largest weight. */

D3ANIMSLOT *d3anim_GetMainSlot(D3ANIMCTL *ctl)
{
    float best = 0.0f;
    D3ANIMSLOT *s = 0;
    int i;

    if (ctl->active) {
        for (i = 2; i >= 0; i--) {
            if (ctl->slots[i].data && best < ctl->slots[i].weight) {
                best = ctl->slots[i].weight;
                s = &ctl->slots[i];
            }
        }
    }
    return s;
}
