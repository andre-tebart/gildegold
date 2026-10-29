/* 0x403620, drags the transport behind the character and puts it on the ground */

void ch_PlaceTransport(CHARACTER *ch, char now)
{
    D3OBJECT *body = ch->obj;
    D3SM *hmap = (D3SM *)ch_GetHMap(ch);
    D3OBJECT *obj;
    int z, x;
    float h2;
    D3VECTOR4 v;
    D3VECTOR4 d;
    float len, k, dx, dy, dz, a;
    D3VECTOR rot;
    int t;
    float h;

    if (!ch->transport)
        return;
    obj = CH_TRANSPORT(ch)->obj;
    if (d3_VecEqual(&body->pos, &CH_TRANSPORT(ch)->pos, 0.01f) && !now)
        return;
    v.x = ch->obj->posOffset.x + ch->obj->pos.x;
    v.y = ch->obj->posOffset.y + ch->obj->pos.y;
    v.z = ch->obj->posOffset.z + ch->obj->pos.z;
    dx = v.x - CH_TRANSPORT(ch)->pos.x;
    dy = v.y - CH_TRANSPORT(ch)->pos.y;
    dz = v.z - CH_TRANSPORT(ch)->pos.z;
    len = (float)sqrt(dx * dx + dy * dy + dz * dz);
    v.x = 0.0f * obj->matrix._11 + 0.0f * obj->matrix._21 + len * obj->matrix._31;
    v.y = 0.0f * obj->matrix._12 + 0.0f * obj->matrix._22 + len * obj->matrix._32;
    v.z = 0.0f * obj->matrix._13 + 0.0f * obj->matrix._23 + len * obj->matrix._33;
    d.x = v.x + obj->pos.x;
    d.y = v.y + obj->pos.y;
    d.z = v.z + obj->pos.z;
    v.x = body->pos.x - d.x;
    v.y = body->pos.y - d.y;
    v.z = body->pos.z - d.z;
    k = (float)(70.0f / (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
    d.x = k * v.x;
    d.y = k * v.y;
    d.z = k * v.z;
    v.x = body->pos.x - d.x;
    v.y = body->pos.y - d.y;
    v.z = body->pos.z - d.z;
    d3_SetObjectPos(obj, (D3VECTOR *)&v);
    a = -(6.2831855f - d3_YawTo(obj, &body->pos));
    rot.x = obj->rotation.x;
    rot.y = obj->rotation.y;
    rot.z = obj->rotation.z;
    rot.y += a;
    d3_SetObjectRot(obj, &rot);
    if (d3_GetHeightAt(hmap, &obj->pos, &x, &z, &h)) {
        t = hmap->entries[hmap->size * z + x].type;
        if ((t == 0 || t == 0xd) && !ch->scene->map)
            goto done;
        if (ch->scene->map)
            map_Pick(ch->scene->map, &obj->pos, 1, &x, &z, &h2);
        if (t == 0 || t == 0xd || !(h2 - h < -5.0))
            h = h2;
        if (!d3t_SceneIndex(ch->scene))
            h += 3.0f;
        if (now)
            obj->pos.y = h;
        else
            obj->pos.y = (h - obj->pos.y) * 0.25f + obj->pos.y;
    }
    d3_SetObjectPos(obj, &obj->pos);
done:
    CH_TRANSPORT(ch)->pos.x = body->pos.x;
    CH_TRANSPORT(ch)->pos.y = body->pos.y;
    CH_TRANSPORT(ch)->pos.z = body->pos.z;
}
