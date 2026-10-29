/* 0x46e190, window (type 0x40 object) under the point, -1 if there is none */

int d2_FindWindowAt(short x, short y)
{
    int i, busy;
    D2OBJ *o;

    d2_PointerWin = -1;
    for (i = 0x2ff; i >= 0; i--) {
        busy = 0;
        o = d2_DrawOrder[i];
        if (o) {
            if (o->buf)
                busy = o->buf->loaded;
            if (x >= o->x && x <= o->x + o->w && y >= o->y && y <= o->y + o->h && !busy
                && y >= o->clipy0 && y <= o->clipy1 && !o->clickDisabled && !o->disabled && d2_Objs[o->id].type == 0x40) {
                d2_PointerWin = o->input;
                break;
            }
        }
    }
    return d2_PointerWin;
}
