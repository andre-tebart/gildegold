/* 0x406210, action callback: turns towards the object args[0] */

void ch_ActTurnTo(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3OBJECT *obj = (D3OBJECT *)a->args[0];
    float ang;
    D3VECTOR rot, from, to;
    D3VECTOR4 target;

    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&target);
    rot.y = (ang = d3_YawTo(ch->obj, (D3VECTOR *)&target));
    rot.x = 0.0f;
    rot.z = 0.0f;
    d3_UnwrapAngles(&rot, &ch->obj->rotation, &from, &to);
    rot.x += ch->obj->rotation.x;
    rot.y += ch->obj->rotation.y;
    rot.z += ch->obj->rotation.z;
    ch_InsertAction(ch, a, 7, (int)(ang * 57.295776f), 0);
    ch_RemoveAction(a);
}
