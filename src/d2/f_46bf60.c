/* 0x46bf60, draws a button: left cap, right cap, the middle tiled, then the caption @deep B draft: 222 instructions, 16 differ (26 code hunks) - register allocation / member load order, no meaning error. (1) 0x46c039..0x46c059 len = w->w - r->w - l->w: original subtracts l->w first (ecx = [esp+0x40] = l, ebx = r), ours r first; source form w->w - l->w - r->w compiles to identical bytes (VC6 reassociates). (2) 0x46c091 / 0x46c0a3 w->w - r->w + w->x: original loads w->w (ecx) then w->x (edx), ours w->x then w->w; the commuted spelling w->x + w->w - r->w compiles to identical bytes. spell.py: one 'reload' prescription at 0x46c091 (drop the local) - the reload is already equal on both sides (asmdiff shows the same mov eax,[esp+0x48]), so it does not touch the differing loads. Recorded attempts (notes/units/d2.md [d2j] Member load order, [z3]): ~21 variants on this function (dead stores, dead member loads into reused locals, d2_Objs[id]. form, frame pointer array, casts via 0x47c6f0, renaming l/r, declaration order) and ~60 on the siblings, plus this session's two source orders - all no change. Readability done (d2_DrawButton; D2OBJ.checked/clickDisabled/greyed/textShadow/rightAligned; callees named). */

void d2_DrawButton(int id, D2SURF *surf)
{
    D2OBJ *w;
    D2FRAME *l, *r, *m;
    unsigned int style;
    void *sprites;
    int len, i;
    int frame = 0;
    int color = 8;

    w = &d2_Objs[id];
    style = w->style;
    if ((style & 2) && !d2_FocusedInput) {
        if ((&d2_Objs[d2_PointerObj] == w || w->checked) && !w->clickDisabled) {
            if (style & 1)
                w->highlighted = 1;
        } else if (style & 1)
            w->highlighted = 0;
    }
    if (!(style & 2) && w->checkable && !w->clickDisabled)
        w->highlighted = w->checked;
    if (w->highlighted)
        color = 10;
    else if (w->greyed)
        color = 9;
    d2_SetFont(w->font);
    sprites = w->data;
    l = d2_GetFrame(sprites, 0);
    r = d2_GetFrame(sprites, 1);
    m = d2_GetFrame(sprites, 2);
    len = w->w - r->w - l->w;
    if (w->highlighted)
        frame = 3;
    d2_DrawShape(w->x, w->y, surf, sprites, frame);
    d2_DrawShape(w->w - r->w + w->x, w->y, surf, sprites, frame + 1);
    d2_SetClipRect(w->x + l->w - 1, d2_ClipTop, w->x + l->w + len, d2_ClipBottom);
    for (i = 0; i < len / m->w + 0.5; i++)
        d2_DrawShape(m->w * i + w->x + l->w, w->y, surf, sprites, frame + 2);
    d2_DrawTextBox(w->x, (l->h >> 1) - d2_FontHeight / 2 + 1 + w->y, w->w, w->h, surf, w->u.text, color);
}
