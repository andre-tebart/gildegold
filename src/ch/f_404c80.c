/* 0x404c80, camera position and angles for view 0..3 relative to the character */

void ch_GetCamera(CHARACTER *ch, int view, D3VECTOR4 *pos, D3VECTOR *rot)
{
    if (!ch)
        return;
    switch (view) {
    case 0:
        pos->x = -3.0f;
        pos->y = 63.0f;
        pos->z = 36.0f;
        rot->y = 3.1415927f;
        rot->x = 0.0f;
        rot->z = 0.0f;
        break;
    case 1:
        pos->x = -10.0f;
        pos->y = 63.0f;
        pos->z = -14.0f;
        rot->x = 0.08726647f;
        rot->y = 0.0f;
        rot->z = 0.0f;
        break;
    case 2:
        pos->x = 10.0f;
        pos->y = 63.0f;
        pos->z = -14.0f;
        rot->x = 0.08726647f;
        rot->y = 0.0f;
        rot->z = 0.0f;
        break;
    case 3:
        pos->x = 0.0f;
        pos->y = 65.0f;
        pos->z = 0.0f;
        rot->y = 0.0f;
        rot->x = 0.0f;
        rot->z = 0.0f;
        break;
    }
    if (ch->flags.bytes.lo & 0x10)
        pos->y -= 15.0;
    d3_LocalToWorld(ch->obj, (D3VECTOR *)pos, (D3VECTOR *)pos);
    rot->x += ch->obj->rotation.x;
    rot->y += ch->obj->rotation.y;
    rot->z += ch->obj->rotation.z;
}
