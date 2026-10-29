/* 0x405260, up to 16 other characters of the same scene within dist of ch */

int ch_FindNear(CHARACTER *ch, CHARACTER **out, float dist)
{
    int i, n = 0;
    CHARACTER *o;

    for (i = 0; i < CH_MAX; i++) {
        if (n >= 16)
            break;
        o = ch_List[i];
        if (o && ch != o && ch->scene == o->scene && ch->group == o->group
            && (!CH_ACTIONS(o) || CH_ACTIONS(o)->type != '-')
            && (o->obj->type != 1 || o->lowPolyBody && o->lowPolyBody->type != 1)
            && d3_VecEqual(&ch->obj->pos, &o->obj->pos, dist))
            out[n++] = ch_List[i];
    }
    return n;
}
