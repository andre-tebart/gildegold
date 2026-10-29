/* 0x477ba0, mouse and keys on the focused slider object @deep B3: 134 code + 4 slots hunks (20 fpu / 5 fill explained); read the body against 0x477ba0..0x477ec0: same flow (focus d2_Unk5febec, type 0x45, flags & 0x40, buf->unk198 picks d2_PointerX/0c, the ldown block loads f0/f6 and the six fields min/max/flags/len/value2, the fl & 0x48 / 2 / 1 branches with mn/v2/mx clamps, wheel: lclick/lhold, max < 100, hilo/hihi inc/dec, same clamps; the original spills flags/max/min/len/value2/range to six stack slots ([esp+0x10] keeps fl as a byte, read at the wheel) and takes edi from the entry, ours keeps max in ebx/ebp; the unit note records that all 120 orders of the five field loads were tried with no change. No meaning error found */

void d2_SliderInput(void)
{
    D2OBJ *o;
    D2FRAME *f0, *f6;
    void *h;
    int px, py, v, mn, mx, len, v2, d, a;
    unsigned int fl;

    if (dm_L.ldown == 0 || d2_Unk5febec == -1)
        d2_Unk5febec = -1;
    else
        d2_PointerObj = d2_Unk5febec;
    if (d2_PointerObj == -1)
        return;
    o = &d2_Objs[d2_PointerObj];
    if (o->type != 0x45)
        return;
    if (o->u.s.flags & 0x40)
        return;
    if (o->buf == 0 || o->buf->loaded == 0) {
        px = dm_L.x;
        py = dm_L.y;
    } else {
        px = d2_PointerX;
        py = d2_PointerY;
    }
    if (dm_L.ldown != 0) {
        h = o->data;
        f0 = d2_GetFrame(h, 0);
        d2_GetFrame(h, 1);
        d2_GetFrame(h, 3);
        f6 = d2_GetFrame(h, 6);
        o = &d2_Objs[d2_PointerObj];
        fl = o->u.s.flags;
        mx = o->u.s.max;
        mn = o->u.s.min;
        len = o->u.s.len;
        v2 = o->u.s.value2;
        d = mx - mn;
        d2_Unk5febec = d2_PointerObj;
        if ((fl & 0x48) == 0) {
            if ((fl & 2) && px > f6->w + o->x && px < o->w - f6->w + o->x) {
                a = px - f0->w - o->x;
                v = (int)(a * ((double)d / len) + mn + 0.5);
            } else if ((fl & 1) && py > f6->h + o->y && py < o->y + o->h - f6->h) {
                a = o->y + o->h - f0->h - py;
                v = (int)(a * ((double)d / len) + mn + 0.5);
            } else {
                goto wheel;
            }
            if (v < mn)
                v = mn;
            else if ((fl & 0x10) && v > v2)
                v = v2;
            else if (v > mx)
                v = mx;
            o->dirty = 1;
            d2_Objs[d2_PointerObj].u.s.value = v;
        }
    }
wheel:
    if (dm_L.lclick == 0) {
        if (dm_L.lhold == 0)
            return;
        if (d2_Objs[d2_PointerObj].u.s.max < 100)
            return;
    }
    mx = d2_Objs[d2_PointerObj].u.s.max;
    mn = d2_Objs[d2_PointerObj].u.s.min;
    o = &d2_Objs[d2_PointerObj];
    v = o->u.s.value;
    if ((fl & 2) == 0) {
        if (o->hilo != 0)
            v++;
        else if (o->hihi != 0)
            v--;
    } else {
        if (o->hilo != 0)
            v--;
        else if (o->hihi != 0)
            v++;
    }
    if (v < mn)
        v = mn;
    else if ((fl & 0x10) && v > v2)
        v = v2;
    else if (v > mx)
        v = mx;
    o->dirty = 1;
    d2_Objs[d2_PointerObj].u.s.value = v;
}
