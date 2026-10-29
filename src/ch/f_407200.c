/* 0x407200, up to 16 other characters of the same scene within 300 units of ch */

int ch_FindNear300(CHARACTER *ch, CHARACTER **out)
{
    int i, n = 0;
    CHARACTER *o;

    for (i = 0; i < CH_MAX; i++) {
        if (n >= 16)
            break;
        o = ch_List[i];
        if (o && ch != o && ch->scene == o->scene && ch->group == o->group
            && (o->obj->type != 1 || o->lowPolyBody && o->lowPolyBody->type != 1)
            && d3_VecEqual(&ch->obj->pos, &o->obj->pos, 300.0f))
            out[n++] = ch_List[i];
    }
    return n;
}
