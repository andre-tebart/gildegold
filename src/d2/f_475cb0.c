/* 0x475cb0, sets the value (and range) of an input / slider / counter object */

void d2_SetObjValue(int obj, int a, int b, int c, int d)
{
    D2OBJ *o;
    D2INPUT *in;
    int k, mn, mx, v;
    unsigned int fl;

    if (obj == -1)
        return;
    if (obj > 0x300)
        return;
    o = &d2_Objs[obj];
    if (o->type == 0x41) {
        in = (D2INPUT *)o->data;
        if (in->flags & 1) {
            if (strlen((char *)a) > 0xfe)
                return;
            strcpy(in->text, (char *)a);
            return;
        }
        if (in->flags & 2) {
            in->minval = a;
            in->maxval = b;
            in->value = c;
            _itoa(c, in->text, 10);
            fl = in->flags;
            if (!(fl & 0x80))
                return;
            k = d2_InputStep(in);
            in->maxval = in->maxval - (in->maxval - in->minval) % k;
            in->minval = (in->maxval - in->minval) % k + in->minval;
            in->value = (in->value / k) * k;
            if ((in->maxval - in->minval) / k > 0x19)
                in->maxval = k * 0x19;
            if (in->value == -1 && (fl & 0x40))
                return;
            v = in->minval;
            if (in->minval <= in->value)
                v = in->value;
            in->value = v;
            return;
        }
    }
    if (o->type == 0x45) {
        o->u.s.min = a;
        o->u.s.max = b;
        o->u.s.value = c;
        fl = o->u.s.flags;
        if (fl & 0x10) {
            o->u.s.value2 = d;
            return;
        }
    } else if (o->checkable != 0 || o->interactive != 0) {
        o->checked = a & 0xff;
        o->highlighted = a & 0xff;
    }
}
