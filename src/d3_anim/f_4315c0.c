/* 0x4315c0, applies the root motion between two key frames: adds the frames' move delta (flags bit 0), in
   object space, to the object position, and the turn delta (flags bit 1) to its rotation. No return value */
void d3anim_ApplyRootMotion(D3OBJECT *obj, D3ANIMSLOT *s, int from, int to, int flags)
{
    D3VECTOR4 d, v;

    if ((flags & 1) == 1) {
        d.x = s->data->frames[to].move.x - s->data->frames[from].move.x;
        d.y = s->data->frames[to].move.y - s->data->frames[from].move.y;
        d.z = s->data->frames[to].move.z - s->data->frames[from].move.z;
        v.x = d.x * obj->matrix._11 + d.y * obj->matrix._21 + d.z * obj->matrix._31;
        v.y = d.x * obj->matrix._12 + d.y * obj->matrix._22 + d.z * obj->matrix._32;
        v.z = d.x * obj->matrix._13 + d.y * obj->matrix._23 + d.z * obj->matrix._33;
        v.x += obj->pos.x;
        v.y += obj->pos.y;
        v.z += obj->pos.z;
        d3_SetObjectPos(obj, (D3VECTOR *)&v);
    }
    if ((flags & 2) == 2) {
        d.x = s->data->frames[to].turn.x - s->data->frames[from].turn.x;
        d.y = s->data->frames[to].turn.y - s->data->frames[from].turn.y;
        d.z = s->data->frames[to].turn.z - s->data->frames[from].turn.z;
        d.x += obj->rotation.x;
        d.y += obj->rotation.y;
        d.z += obj->rotation.z;
        d3_SetObjectRot(obj, (D3VECTOR *)&d);
    }
}
