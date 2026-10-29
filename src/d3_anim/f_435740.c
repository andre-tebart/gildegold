/* 0x435740, stops the animation name. */

void d3_ReleaseTexture(void *list, char *name)
{
    D3ANIMCTL *ctl = list;
    D3ANIMOBJ *obj;
    int i;

    if (ctl) {
        obj = ctl->obj;
        for (i = 0; i < 3; i++) {
            if (ctl->slots[i].data && !strcmp(name, ctl->slots[i].data->name)) {
                ctl->slots[i].flags.b[8] &= ~2;
                d3anim_Release(ctl->slots[i].data, 0);
                ctl->slots[i].data = 0;
            }
        }
        for (i = 0; i < 3; i++)
            if (ctl->slots[i].data)
                break;
        if (i == 3)
            ctl->active = 0;
        d3anim_MapBones((D3OBJECT *)obj);
        d3anim_PoseBones((D3OBJECT *)obj);
    }
}
