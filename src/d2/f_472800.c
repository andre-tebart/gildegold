/* 0x472800, resizes a window (grows in height so that all objects fit) @deep draft, readable (Q1/Q2/Q4 done this session): 438/608 bytes, 29 asm lines, hunks code 40 / order 6 / fill 1 - same instructions, scheduling and operand order only. spell.py: no hunk matched the menu (escalate with the diffclass verdict). Left: 47284c (the delta store moved across the jle), 472877 (the object-loop body loads d2_Objs[].h before wn->h in the original; ours loads wn->h first), 47298a / 472a01 (our o->clipy1 = wn->y + wn->h emits y then h, the original h then y; o->clipx1 similarly), 472a5f alignment fill. Tried: swapping the two clip expressions to wn->h + wn->y / wn->x + wn->w - identical bytes, reverted. Q4 fixed byte-neutrally here: int *p = wn->objs; *p; p++ instead of the int-as-byte-pointer walk win = (int)wn->objs / *(int *)win / win += 4 (438/608 bytes and 29 lines unchanged). Q1/Q2 clean (named d2_ResizeWindow earlier this session, was d2_ResizeWindow): the typed int *p walk replaced the two *(int *)win raw accesses. */

void d2_ResizeWindow(w, h, win)
short w;
short h;
int win;
{
    D2WIN *wn = &d2_Wins[win];
    int delta, dw, n, i, oh, oy;
    int desc[16];
    int *p;
    D2OBJ *o;
    void *ns, *old;

    if (wn->active) {
        dw = w - wn->w;
        delta = h - wn->h;
        n = wn->nobjs;
        if (n > 0) {
            p = wn->objs;
            do {
                oh = d2_Objs[*p].h;
                oy = d2_Objs[*p].y;
                if (wn->y + wn->h + delta < oy + oh)
                    delta = oy - wn->y - wn->h + 8 + oh;
                p++;
                n--;
            } while (n);
        }
        if (wn->surf0) {
            mem_Free(wn->surf0);
            wn->surf0 = 0;
        }
        if (wn->surf1) {
            mem_Free(wn->surf1);
            wn->surf1 = 0;
        }
        if (wn->surf2) {
            memset(desc, 0, sizeof desc);
            desc[1] = w;
            desc[2] = h;
            ns = d2_MakeSurface(desc, 0xb);
            old = wn->surf2;
            d2_BlitPart(0, 0, ((int *)old)[1], ((int *)old)[2], old, 0, 0, ns);
            d2_FreeSurface(wn->surf2);
            wn->surf2 = ns;
        }
        d2_AddDirty(wn->x, wn->y, wn->w, wn->h);
        wn->h += delta;
        wn->w += dw;
        o = &d2_Objs[wn->parent];
        o->clipy1 = wn->y + wn->h;
        o->clipx1 = wn->w + wn->x;
        if (wn->y + wn->h > d2_ScreenH)
            wn->h = d2_ScreenH - wn->y;
        if (wn->w + wn->x > d2_ScreenW)
            wn->w = d2_ScreenW - wn->x;
        for (i = 0; i < wn->nobjs; i++) {
            o = &d2_Objs[wn->objs[i]];
            o->clipy1 = wn->y + wn->h;
            o->clipx1 = wn->w + wn->x;
        }
        o = &d2_Objs[wn->parent];
        o->w = wn->w;
        o->h = wn->h;
    }
}
