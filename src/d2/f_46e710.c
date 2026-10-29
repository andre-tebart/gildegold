/* 0x46e710, removes object obj: frees what its type owns, unlinks it from its window and the draw list,
   marks its rectangle dirty and clears the entry
   @deep draft (105 differing instructions of 260 vs 263; 402/656 bytes). The stream is the same statements under another register map: ours materializes the constant 0 once in ebp and reuses it for the stores (mov [x+0x3b4],ebp / mov [x+0x7c],ebp / mov [x+0xbfc],ebp) and the compares (cmp eax,ebp), the original stores immediates (mov [x+0xbfc],0x0) and keeps its zero in edi (cmp eax,edi, test edi,edi); the object-cursor computation (46e724) and the window object scan (46e816: ours increments a pointer, the original indexes [edx+eax*4-0x4], both the i++/i-- find loop) are the same operations. spell.py reload at 46e710 (drop the off local, re-read obj at the memset and the id write) tried: identical bytes, reverted; dup-tail at 46e7c9 is the same call's argument pushed from another register, no repeated source statement; code at 46e724/46e78f are the address computations, registers only. No meaning error found (the loop bounds, the memmove, d2_HighestObj and the final id write read equal). Readability done (Q1..Q7: sizedObjCount field, typed id write). Left: the constant-0 register choice and the cascade (listing/c2prio/siblings). */

void d2_RemoveObj(int obj)
{
    D2OBJ *o;
    D2WIN *w;
    D2INPUT *in;
    int i, n, off;

    off = obj * sizeof(D2OBJ);
    o = (D2OBJ *)((char *)d2_Objs + off);
    if (!o->used)
        return;
    switch (o->type) {
    case 0x43:
        if (o->input)
            mem_Free(o->input);
        break;
    case 4:
        d2_FreeAnim(o->input);
        break;
    case 0x40:
        d2_CloseWindow(o->input);
        break;
    case 0x41:
        in = &d2_Inputs[o->input];
        if (d2_FocusedInput == in)
            d2_FocusedInput = 0;
        if (in->icon != -1)
            d2_SubRefcount(in->icon);
        memset(in, 0, sizeof(D2INPUT));
        break;
    case 0x45:
        d2_FreeSurface(o->u.s.img);
        break;
    case 9:
        if (o->win)
            o->win->sizedObjCount = 0;
        break;
    case 0x47:
        if (o->u.raw) {
            mem_Free(o->u.raw);
            o->u.raw = 0;
        }
        break;
    }
    if (o->type < 0x40)
        d2_SubRefcount(o->pic);
    w = o->win;
    if (w) {
        n = w->nobjs;
        for (i = 0; i < n;)
            if (w->objs[i++] == obj)
                break;
        i--;
        if (i != n)
            memmove(&w->objs[i], &w->objs[i + 1], (n - i) * 4);
        w->nobjs--;
    }
    if (o->buf) {
        if (o->buf->loaded)
            d2_AddShape(o->x, o->y, o->w, o->h, (D2BUF *)o->buf);
        else
            d2_AddDirty(o->x, o->y, o->w, o->h);
    } else
        d2_AddDirty(o->x, o->y, o->w, o->h);
    for (i = 0; i < 0x2ff; i++) {
        if (d2_DrawOrder[i] == o) {
            if (i == 0x2ff)
                d2_DrawOrder[0x2ff] = 0;
            else
                memmove(&d2_DrawOrder[i], &d2_DrawOrder[i + 1], (0x2fe - i) * 4);
            break;
        }
    }
    if (obj == d2_HighestObj)
        d2_HighestObj = obj - 1;
    memset((char *)d2_Objs + off, 0, sizeof(D2OBJ));
    ((D2OBJ *)((char *)d2_Objs + off))->id = obj;
}
