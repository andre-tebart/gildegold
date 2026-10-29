/* 0x408e60, walk actions: avoids other characters, then takes the next path cell; 0 when the walk ends */

int ch_NextWaypoint(CHACTION *a, D3SM *h)
{
    CHARACTER *ch = a->ch;
    int n, r;
    float speed, f, dx, dy, dz;
    D3OBJECT *o;
    CHARACTER *others[16];

    if (ch->contactSteps < 1 && (ch->kind & 1) && (!ch->slot || !CH_XSLOT(ch)->f74_8)
        && (d3_DefaultScene == ch->scene || ch->scene == d3_Scenes)) {
        if (n = ch_FindNear300(ch, others)) {
            r = ch_GetNewPath(ch, n, others);
            if (r > 0) {
                if (ch->slot) {
                    if (ch->kind & 8)
                        speed = 30.0f;
                    else if (ch->kind & 0x10)
                        speed = 30.0f;
                    else
                        speed = 15.0f;
                    if (ch->scene->map)
                        speed *= 3.0;
                    f = 50.0f / h->cell.x;
                    CH_XSLOT(ch)->speed = speed * min(2.0f, f);
                    CH_XSLOT(ch)->play = 1;
                    CH_XSLOT(ch)->f74_8 = 0;
                    CH_XSLOT(ch)->f74_4 = 1;
                    CH_XSLOT(ch)->f6d_20 = 0;
                    o = a->ch->obj;
                    dx = o->pos.x - a->u.walk.end.x;
                    dy = o->pos.y - a->u.walk.end.y;
                    dz = o->pos.z - a->u.walk.end.z;
                    f = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    a->u.walk.dist = f;
                }
            } else if (r == -1) {
                if (ch->slot)
                    CH_XSLOT(ch)->playSpeed = 0.01f;
                a->u.walk.retryAt = ch_Time;
                return 0;
            } else if (r == -2) {
                ch_StopAnim(ch);
                if (ch_Unk631058)
                    a->ch->flags.bytes.lo |= 8;
                if (a->u.walk.path) {
                    mem_Free(a->u.walk.path);
                    a->u.walk.path = 0;
                }
                ch_RemoveAction(a);
                return 0;
            }
        }
    }
    if (!a->u.walk.cur || (ch->slot && CH_XSLOT(ch)->f74_8)) {
        if ((a->u.walk.flags & CH_WALK_TARGET) && a->u.walk.cur % 4 == 0
            && !d3_VecEqual(&a->u.walk.tpos, &a->u.walk.target->obj->pos, 70.0f))
            ch_PathToTarget(a);
        a->u.walk.cur++;
        if (ch->contactSteps > 0 && !--ch->contactSteps)
            memset(ch->contacts, 0, sizeof ch->contacts);
        if (a->u.walk.cur >= a->u.walk.pathLen) {
            if (a->u.walk.path) {
                mem_Free(a->u.walk.path);
                a->u.walk.path = 0;
            }
            if (a->u.walk.flags & CH_WALK_DEST) {
                if (ch->scene == d3_Scenes && a->u.walk.retries > 4)
                    a->u.walk.flags &= ~CH_WALK_DEST;
                if (!d3_VecEqual(&ch->obj->pos, &a->u.walk.dest, 5.0f)) {
                    a->u.walk.end.x = a->u.walk.dest.x;
                    a->u.walk.end.y = a->u.walk.dest.y;
                    a->u.walk.end.z = a->u.walk.dest.z;
                    CH_XSLOT(ch)->pos.x = a->u.walk.end.x;
                    CH_XSLOT(ch)->pos.y = a->u.walk.end.y;
                    CH_XSLOT(ch)->pos.z = a->u.walk.end.z;
                    o = a->ch->obj;
                    dx = o->pos.x - a->u.walk.end.x;
                    dy = o->pos.y - a->u.walk.end.y;
                    dz = o->pos.z - a->u.walk.end.z;
                    a->u.walk.angle = -1.0f;
                    f = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    a->u.walk.dist = f;
                    CH_XSLOT(ch)->speed = 5.0f;
                    CH_XSLOT(ch)->play = 1;
                    CH_XSLOT(ch)->f74_8 = 0;
                    CH_XSLOT(ch)->f6d_20 = 0;
                    a->u.walk.retries++;
                    return 0;
                }
                d3_SetObjectPos(ch->obj, &a->u.walk.dest);
                ch_PlaceOnGround(ch, 1);
            }
            ch_StopAnim(ch);
            if (ch_Unk631058)
                a->ch->flags.bytes.lo |= 8;
            ch_RemoveAction(a);
            return 0;
        }
        d3sm_GetVertex(h, ((unsigned char *)a->u.walk.path)[a->u.walk.cur * 2],
                       ((unsigned char *)a->u.walk.path)[a->u.walk.cur * 2 + 1], &a->u.walk.end);
        a->u.walk.end.x += ch_PathOffset.x;
        a->u.walk.end.y += ch_PathOffset.y;
        a->u.walk.end.z += ch_PathOffset.z;
        a->u.walk.angle = d3_YawTo(ch->obj, &a->u.walk.end);
        o = a->ch->obj;
        dx = o->pos.x - a->u.walk.end.x;
        dy = o->pos.y - a->u.walk.end.y;
        dz = o->pos.z - a->u.walk.end.z;
        f = (float)sqrt(dx * dx + dy * dy + dz * dz);
        a->u.walk.dist = f;
        if (ch->slot) {
            if (a->u.walk.cur >= a->u.walk.pathLen - 1 && a->u.walk.pathLen > 1) {
                if (ch->kind & 8)
                    speed = 24.0f;
                else if (ch->kind & 0x10)
                    speed = 64.0f;
                else
                    speed = 8.0f;
            } else {
                if (ch->kind & 8)
                    speed = 45.0f;
                else if (ch->kind & 0x10)
                    speed = 75.0f;
                else
                    speed = 15.0f;
            }
            if (ch->scene->map)
                speed *= 3.0;
            CH_XSLOT(ch)->pos.x = a->u.walk.end.x;
            CH_XSLOT(ch)->pos.y = a->u.walk.end.y;
            CH_XSLOT(ch)->pos.z = a->u.walk.end.z;
            CH_XSLOT(ch)->speed = speed;
            CH_XSLOT(ch)->play = 1;
            CH_XSLOT(ch)->f74_8 = 0;
            CH_XSLOT(ch)->f6d_20 = 0;
        }
    }
    return 1;
}
