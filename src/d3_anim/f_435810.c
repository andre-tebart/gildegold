/* 0x435810, pauses the slot that plays the animation name: clears its play flag (the slot and its data
   stay loaded; ch_PauseAnim calls this with pause = 1). */

void d3anim_PauseAnim(D3ANIMCTL *ctl, char *name)
{
    int i;

    if (ctl) {
        for (i = 0; i < 3; i++)
            if (ctl->slots[i].data && !strcmp(name, ctl->slots[i].data->name))
                ctl->slots[i].flags.b[8] &= ~2;
    }
}
