/* 0x46a110, opens a number input without icon @deep Q2 only: D2INPUT.unk144 is written once (in->unk144 = 1 beside mult = 1 in d2_OpenInput's slot setup; Ghidra 0x46a110 stores 1 at D2INPUT+0x144) and has no reader in the drafted sources, no string, caller or data evidence and no header comment - a name would be invented; the other identifiers cleared with D2OBJ.hidden -> hidden */

int d2_OpenInput(int x, int y, int w, int h, unsigned char flags)
{
    int i;
    int obj;
    D2INPUT *in;
    int n, v;
    char fmt[8];
    char buf[16];

    i = 1;
    if (d2_Inputs[1].used) {
        while (d2_Inputs[++i].used)
            ;
        if (i > 0x7f) {
            err_Message("d2_OpenInputIcon(): Too many inputs on screen...");
            return -1;
        }
    }
    in = &d2_Inputs[i];
    in->id = i;
    in->x = x;
    in->y = y;
    in->w = w;
    in->h = h;
    in->maxval = 999;
    in->minval = 0;
    in->value = 0;
    in->mult = 1;
    in->unk144 = 1;
    in->ndigits = 3;
    in->flags = flags;
    in->used = 1;
    in->font = d2_FindPic("_FONT");
    in->icon = -1;
    in->buf = 0;
    obj = d2_NewObject();
    if (flags & 2) {
        n = 0;
        v = in->maxval;
        do {
            if (v <= 0)
                break;
            v /= 10;
            n++;
        } while (n < 10);
        in->ndigits = n;
        sprintf(fmt, "%%0%ii", in->ndigits);
        sprintf(buf, fmt, in->value * in->mult);
        in->w = d2_TextWidth(buf, in->font);
    }
    if (flags & 1)
        in->maxval = 0x20;
    if (flags & 0x30)
        d2_Objs[obj].hidden = 1;
    d2_Objs[obj].type = 0x41;
    d2_Objs[obj].x = in->x;
    d2_Objs[obj].y = in->y;
    d2_Objs[obj].w = in->w;
    d2_Objs[obj].h = in->h;
    d2_Objs[obj].pic = 0;
    d2_Objs[obj].input = in->id;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].clipy0 = in->y;
    d2_Objs[obj].clipx0 = in->x;
    d2_Objs[obj].clipx1 = in->w + in->x;
    d2_Objs[obj].clipy1 = in->h + in->y;
    d2_Objs[obj].data = in;
    in->obj = obj;
    return obj;
}
