/* 0x475980, value of an input / slider / counter object; -1 if it has none */

int d2_GetObjValue(int obj)
{
    D2OBJ *o = &d2_Objs[obj];
    D2INPUT *in;

    if (o->type == 0x41) {
        in = (D2INPUT *)o->data;
        if (in->flags & 0x10)
            return in->currentTextId;
        if (in->flags & 1)
            return (int)in->text;
        if (in->flags & 2)
            return in->value;
    }
    if (o->checkable)
        return o->checked;
    if (o->type == 0x45)
        return o->u.s.value;
    return -1;
}
