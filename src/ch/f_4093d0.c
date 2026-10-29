/* 0x4093d0, walk actions: turns the character towards the next path cell; a->u.walk.angle < 0 left,
   > 0 right; 0: checks that the character did not walk past the cell */

void ch_WalkRotate(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3OBJECT *o;
    float ang, r, step;
    double rot;
    D3VECTOR4 d, p;
    char buf[0x100];

    if (a->u.walk.angle < 0.0f) {
        o = ch->obj;
        ang = d3_YawTo(o, &a->u.walk.end);
        rot = o->rotation.y;
        a->u.walk.angle = ang;
        if (!(ang <= 0.04 && ang >= -0.04)) {
            step = (ch_Time - ch_LastUpdate) / 1.1f;
            if (ch->kind & 8)
                step *= 0.4f;
            else if (ch->kind & 0x10)
                step *= 1.4f;
            if (fabs(ang) > 2.0943952f && !(ch->kind & 8))
                ch->speedScale = 0.4f;
            if (fabs(ang) > 1.5707964f)
                step *= 0.19999999f;
            else if (fabs(ang) > 1.0471976f)
                step *= 0.14f;
            else if (fabs(ang) > 0.5235988f)
                step *= 0.06f;
            else
                step *= 0.04f;
            r = rot - step;
            r = (float)fmod(r, 6.2831855f);
            if (r < 0.0)
                r += 6.2831855f;
            if (r < ang + o->rotation.y) {
                r = ang + o->rotation.y;
                a->u.walk.angle = 0.0f;
            }
        } else {
            r = ang + o->rotation.y;
            a->u.walk.angle = 0.0f;
        }
        d3_SetObjectRotXYZ(o, o->rotation.x, r, o->rotation.z);
    } else if (a->u.walk.angle > 0.0f) {
        o = ch->obj;
        ang = d3_YawTo(o, &a->u.walk.end);
        rot = o->rotation.y;
        a->u.walk.angle = ang;
        if (!(ang <= 0.04 && ang >= -0.04)) {
            step = (ch_Time - ch_LastUpdate) / 1.1f;
            if (ch->kind & 8)
                step *= 0.4f;
            else if (ch->kind & 0x10)
                step *= 0.8f;
            if (fabs(ang) > 2.0943952f && !(ch->kind & 8))
                ch->speedScale = 0.4f;
            if (fabs(ang) > 1.5707964f)
                step *= 0.19999999f;
            else if (fabs(ang) > 1.0471976f)
                step *= 0.14f;
            else if (fabs(ang) > 0.5235988f)
                step *= 0.06f;
            else
                step *= 0.04f;
            r = rot + step;
            r = (float)fmod(r, 6.2831855f);
            if (r < 0.0)
                r += 6.2831855f;
            if (r > ang + o->rotation.y) {
                r = ang + o->rotation.y;
                a->u.walk.angle = 0.0f;
            }
        } else {
            r = ang + o->rotation.y;
            a->u.walk.angle = 0.0f;
        }
        d3_SetObjectRotXYZ(o, o->rotation.x, r, o->rotation.z);
    } else {
        o = ch->obj;
        p.x = o->pos.x;
        p.y = o->pos.y;
        p.z = o->pos.z;
        d.x = p.x - a->u.walk.end.x;
        d.y = p.y - a->u.walk.end.y;
        d.z = p.z - a->u.walk.end.z;
        if ((float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) > a->u.walk.dist * 1.4) {
            if (a->u.walk.cur >= a->u.walk.pathLen - 1)
                ch_SetPos(ch, &a->u.walk.end, 0);
            if (ch->slot)
                CH_XSLOT(ch)->f74_8 = 1;
            sprintf(buf, "ch_WalkOnPathRotation(): Have missed my treshhold... %s [%f %f]", a->ch->obj,
                    a->u.walk.end.x, a->u.walk.end.z);
        }
    }
}
