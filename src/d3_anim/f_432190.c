/* 0x432190, the object's position at frame/sub of the slot's animation (root motion, without moving it). @deep B draft: 407 code hunks + 22 fpu; the recorded attempts in notes/units/d3_anim.md (frames[key] / frames[frame] stack-slot placement, the deliberately uninitialized sum on the unreachable path, pointer locals f0/f1) all failed - a deep matching job, not a base meaning fix. Readability also needs Q1, Q2 (d3_Lerp3, nextFrame/subFrame/relative) and Q4 (D3_OBJOFFSET macro). */

void d3anim_Unk432190(D3OBJECT *obj, D3ANIMSLOT *s, int frame, int sub, D3VECTOR *out)
{
    D3ANIMDATA *data = s->data;
    D3ANIMFRAME *f0, *f1;
    D3VECTOR4 d, sum, v;
    float t;
    int key, sub0, n0, n1, i;

    out->x = 0;
    out->y = 0;
    out->z = 0;
    if (data->relative) {
        n1 = data->frames[frame].count;
        key = s->lastFrame;
        sub0 = s->lastSub;
        n0 = data->frames[key].count;
        if (sub == sub0 && key == frame) {
            out->x = D3_OBJOFFSET(obj).x + obj->pos.x;
            out->y = D3_OBJOFFSET(obj).y + obj->pos.y;
            out->z = D3_OBJOFFSET(obj).z + obj->pos.z;
            return;
        }
        if (key < frame) {
            t = 1.0f - (float)sub0 / n0;
            d.x = data->frames[key + 1].move.x - data->frames[key].move.x;
            d.y = data->frames[key + 1].move.y - data->frames[key].move.y;
            d.z = data->frames[key + 1].move.z - data->frames[key].move.z;
            sum.x = d.x * t;
            sum.y = d.y * t;
            sum.z = d.z * t;
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
            sub -= sub0;
        t = (float)sub / n1;
        d.x = data->frames[frame + 1].move.x - data->frames[frame].move.x;
        d.y = data->frames[frame + 1].move.y - data->frames[frame].move.y;
        d.z = data->frames[frame + 1].move.z - data->frames[frame].move.z;
        d.x *= t;
        d.y *= t;
        d.z *= t;
        sum.x += d.x;
        sum.y += d.y;
        sum.z += d.z;
        v.x = sum.x * obj->matrix._11 + sum.y * obj->matrix._21 + sum.z * obj->matrix._31;
        v.y = sum.x * obj->matrix._12 + sum.y * obj->matrix._22 + sum.z * obj->matrix._32;
        v.z = sum.x * obj->matrix._13 + sum.y * obj->matrix._23 + sum.z * obj->matrix._33;
        out->x = v.x + obj->pos.x;
        out->y = v.y + obj->pos.y;
        out->z = v.z + obj->pos.z;
    } else {
        f0 = &data->frames[s->frame];
        f1 = &data->frames[s->nextFrame];
        d.x = f1->move.x - f0->move.x;
        d.y = f1->move.y - f0->move.y;
        d.z = f1->move.z - f0->move.z;
        t = (float)s->subFrame / f0->count;
        sum.x = d.x * t + f0->move.x;
        sum.y = d.y * t + f0->move.y;
        sum.z = d.z * t + f0->move.z;
        v.x = sum.x * obj->matrix._11 + sum.y * obj->matrix._21 + sum.z * obj->matrix._31;
        v.y = sum.x * obj->matrix._12 + sum.y * obj->matrix._22 + sum.z * obj->matrix._32;
        v.z = sum.x * obj->matrix._13 + sum.y * obj->matrix._23 + sum.z * obj->matrix._33;
        out->x = v.x + obj->pos.x;
        out->y = v.y + obj->pos.y;
        out->z = v.z + obj->pos.z;
    }
    out->x = D3_OBJOFFSET(obj).x + out->x;
    out->y = D3_OBJOFFSET(obj).y + out->y;
    out->z = D3_OBJOFFSET(obj).z + out->z;
}
