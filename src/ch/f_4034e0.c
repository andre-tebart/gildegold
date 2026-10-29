/* 0x4034e0, puts obj 70 units behind the character */

CHTRANSPORT *ch_AttachTransport(CHARACTER *ch, D3OBJECT *obj)
{
    CHTRANSPORT *t = mem_Alloc(sizeof(CHTRANSPORT), "ch:ch_AttachTransport");
    D3VECTOR4 pos;

    pos.x = 0.0f * ch->obj->matrix._11 + 0.0f * ch->obj->matrix._21 - 70.0f * ch->obj->matrix._31 + ch->obj->matrix._41;
    pos.y = 0.0f * ch->obj->matrix._12 + 0.0f * ch->obj->matrix._22 - 70.0f * ch->obj->matrix._32 + ch->obj->matrix._42;
    pos.z = 0.0f * ch->obj->matrix._13 + 0.0f * ch->obj->matrix._23 - 70.0f * ch->obj->matrix._33 + ch->obj->matrix._43;
    d3_SetObjectPos(obj, (D3VECTOR *)&pos);
    d3_SetObjectRot(obj, &ch->obj->rotation);
    obj->flags &= ~4;
    obj->stateFlags = obj->stateFlags & ~8 | 4;
    obj->objectFlags &= ~2;
    ch->transport = (D3OBJECT **)t;
    t->unk14 = 0;
    t->obj = obj;
    return t;
}
