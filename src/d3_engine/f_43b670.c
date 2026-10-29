/* 0x43b670, d3_ForEachObject callback of d3_UpdateObject: marks an object for an update. */

char d3_MarkDirtyCB(D3OBJECT *obj, unsigned char full)
{
    obj->state |= 4;
    if (full)
        obj->stateFlags &= 0x7f;
    obj->flags &= 0xfe;
    return 1;
}
