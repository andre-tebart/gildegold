/* 0x435550, starts the cached animation name in a free slot of the animation control: marks the control
   active, takes a reference on the data (calculating its normals once), sets up frame/nextFrame, speed,
   timeout and the attach fields of the slot and poses the bones. Returns the slot, 0 when there is no free
   slot, the data is missing or the vertex counts differ. */

int d3anim_StartAnim(void *list, char *name, D3ANIMFLAGS f)
{
    D3ANIMCTL *ctl = list;
    D3ANIMDATA *data;
    D3ANIMSLOT *s;
    D3ANIMOBJ *obj;
    int i;

    if (ctl) {
        data = (D3ANIMDATA *)d3_FindAnim(name);
        for (i = 0; i < 3; i++)
            if (!ctl->slots[i].data)
                break;
        if (data && data->nverts == ctl->nverts && i < 3) {
            d3anim_Busy++;
            s = &ctl->slots[i];
            obj = ctl->obj;
            obj->time = d3_Time;
            ctl->active = 1;
            s->data = data;
            s->speed = 1.0f;
            s->frameTime = 0;
            s->timeout = 100;
            data->refs++;
            data->lastuse = d3anim_Clock;
            if (!data->normals && ctl->mesh) {
                d3anim_CalculateAnimNormals(data, ctl->mesh);
                data->normals = 1;
            }
            s->flags = f;
            s->flags.b[1] &= ~0x20;
            s->flags.b[8] = s->flags.b[8] & ~0x48 | 0x10;
            if (f.b[0] == 1)
                s->flags.b[1] |= 0x10;
            s->attachFirst = s->attachLast = s->attachBone = -1;
            s->fadeStart = s->fadeEnd = s->attached = 0;
            s->weight = s->fadeFrom = s->fadeTo = 1.0f;
            s->lastSub = 0;
            if (data->relative)
                s->lastFrame = 0;
            else
                s->lastFrame = data->first;
            if (s->flags.b[1] & 2) {
                s->frame = data->nframes - 1;
                s->subFrame = data->frames[s->frame].count - 1;
                s->nextFrame = d3anim_NextFrame(s->flags, s->frame, data->first, data->last, data->nframes);
            } else {
                s->subFrame = 0;
                s->frame = 0;
                s->nextFrame = 1;
            }
            d3anim_MapBones((D3OBJECT *)obj);
            d3anim_PoseBones((D3OBJECT *)obj);
            d3anim_Busy--;
            return (int)s;
        }
    }
    return 0;
}
