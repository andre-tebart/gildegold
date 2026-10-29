/* 0x435950, sets the slot's current frame (clamped to the last frame) and the sub-frame 0 or the frame's
   end, computes the next frame and marks the object for an update. */

void d3anim_SetFrame(D3OBJECT *obj, D3ANIMSLOT *s, int frame)
{
    D3ANIMDATA *data;

    if (s && (data = s->data)) {
        if (frame >= data->last)
            frame = data->last - 1;
        if (frame < 0)
            frame = data->last - 1;
        if (frame + 1 >= data->last)
            s->subFrame = data->frames[frame].count - 1;
        else
            s->subFrame = 0;
        s->frame = frame;
        s->nextFrame = d3anim_NextFrame(s->flags, frame, data->first, data->last, data->nframes);
        d3_UpdateObject(obj, 1);
    }
}
