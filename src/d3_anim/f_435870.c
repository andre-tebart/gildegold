/* 0x435870, starts or stops the slot of the animation name (its play flag = on) and marks the object's
   animation time as current. */

void d3anim_PlayAnim(D3ANIMCTL *ctl, char *name, char on)
{
    D3ANIMOBJ *obj;
    int i;

    if (ctl) {
        obj = ctl->obj;
        obj->time = d3_Time;
        for (i = 0; i < 3; i++)
            if (ctl->slots[i].data && !strcmp(name, ctl->slots[i].data->name))
                ctl->slots[i].play = on;
    }
}
