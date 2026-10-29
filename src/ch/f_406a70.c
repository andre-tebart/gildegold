/* 0x406a70, keeps the character on the ground; returns the ground type under it */

int ch_FollowGround(CHACTION *a)
{
    D3SM *h;
    CHARACTER *ch;
    D3OBJECT *obj;
    int type;
    int x, z;
    float y, y2;
    D3VECTOR4 pos;

    h = (D3SM *)ch_GetHMap(a->ch);
    ch = a->ch;
    type = 0;
    obj = ch->obj;
    if (h) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        if (d3_GetHeightAt(h, (D3VECTOR *)&pos, &x, &z, &y)) {
            type = h->entries[h->size * z + x].type;
            if (type && type != 13 || ch->scene->map) {
                obj = ch->obj;
                if (ch->scene->map) {
                    map_Pick(ch->scene->map, &obj->pos, 1, &x, &z, &y2);
                    if (!type || type == 13 || !(y2 - y < -5.0))
                        y = y2;
                }
                if (!d3t_SceneIndex(ch->scene))
                    y += 3.0f;
                obj->pos.y += (y - obj->pos.y) * 0.25f;
                d3_SetObjectPos(obj, &obj->pos);
            }
        }
    }
    return type;
}
