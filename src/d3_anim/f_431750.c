/* 0x431750, applies the root-motion translation from the slot's last position (lastFrame/lastSub) up to
   frame/sub (flags bit 0), then stores that position in the slot. No return value */

void d3anim_ApplyMotionTo(D3OBJECT *obj, D3ANIMSLOT *s, int sub, int frame, int flags)
{
    D3ANIMDATA *data = s->data;
    int key = s->lastFrame;
    int sub0 = s->lastSub;
    int n1 = data->frames[frame].count;
    int n0 = data->frames[key].count;
    D3VECTOR4 d, sum;
    float f;
    int i;

    if (sub != sub0 || frame != key) {
        if ((flags & 1) == 1) {
            if (key < frame) {
                f = 1.0f - (float)sub0 / n0;
                d.x = data->frames[key + 1].move.x - data->frames[key].move.x;
                d.y = data->frames[key + 1].move.y - data->frames[key].move.y;
                d.z = data->frames[key + 1].move.z - data->frames[key].move.z;
                sum.x = d.x * f;
                sum.y = d.y * f;
                sum.z = d.z * f;
            } else {
                sum.x = 0.0f;
                sum.y = 0.0f;
                sum.z = 0.0f;
            }
            for (i = key + 1; i < frame; i++) {
                d.x = data->frames[i + 1].move.x - data->frames[i].move.x;
                d.y = data->frames[i + 1].move.y - data->frames[i].move.y;
                d.z = data->frames[i + 1].move.z - data->frames[i].move.z;
                sum.x += d.x;
                sum.y += d.y;
                sum.z += d.z;
            }
            if (key == frame)
                f = (float)(sub - sub0) / n1;
            else
                f = (float)sub / n1;
            d.x = data->frames[frame + 1].move.x - data->frames[frame].move.x;
            d.y = data->frames[frame + 1].move.y - data->frames[frame].move.y;
            d.z = data->frames[frame + 1].move.z - data->frames[frame].move.z;
            d.x *= f;
            d.y *= f;
            d.z *= f;
            sum.x += d.x;
            sum.y += d.y;
            sum.z += d.z;
            d.x = sum.x * obj->matrix._11 + sum.y * obj->matrix._21 + sum.z * obj->matrix._31;
            d.y = sum.x * obj->matrix._12 + sum.y * obj->matrix._22 + sum.z * obj->matrix._32;
            d.z = sum.x * obj->matrix._13 + sum.y * obj->matrix._23 + sum.z * obj->matrix._33;
            d.x += obj->pos.x;
            d.y += obj->pos.y;
            d.z += obj->pos.z;
            d3_SetObjectPos(obj, (D3VECTOR *)&d);
        }
        s->lastFrame = frame;
        s->lastSub = sub;
    }
}
