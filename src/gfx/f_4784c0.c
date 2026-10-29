/* 0x4784c0 @deep B3, 257 code hunks / 602 of 896 bytes, register allocation: the original hoists no zero register (test / mov esi,[mem]; test esi,esi where ours keeps 0 in edi and uses cmp [mem],edi) and the mouse / hit loads repeat differently; the flow, branches and stores read the same (the old note's fit: swapped branches, comma-expression call argument, do-while radio loop); readability is also open here: Q2 (27 mouse / hit table identifiers), Q4 (2 casts) and Q7 (d2_SetObjValue, d3io_NoopCallback declared K&R) */

/* 0x4784c0, mouse handling: object and window under the pointer, clicks (sound, radio groups, toggles), the hit
   rectangles, dragging the scroll bar of a window */
#define RAD(i) ((D2RADIO *)((char *)d2_Radios + (i)))
void d2_MouseUpdate(void)
{
    D2OBJ *o;
    D2CTRLX *c;
    D2HIT *h;
    D2WIN *w;
    int *p, *q;
    int i, j, k, m, n, found, win, v, line;
    short mx, my;

    d2_Unkf8aacc = d2_HitTest(d2_MouseX, d2_MouseY);
    d2_UpdateInput();
    if (d2_PointerWin != -1) {
        d2_HitWindow = d2_PointerWin;
        d2_Unkf8aa80 = -1;
        for (i = 0; i < d2_Wins[d2_PointerWin].nobjs; i++) {
            if (d2_Wins[d2_PointerWin].objs[i] == d2_PointerObj) {
                d2_Unkf8aa80 = i;
                break;
            }
        }
    } else {
        d2_HitWindow = -1;
    }
    d2_Unk477000();
    if (dm_L.lclick != 0) {
        k = d2_PointerObj;
        if (d2_Unkf8aacc != -1 && ((o = &d2_Objs[d2_PointerObj])->checkable != 0 || o->interactive != 0) && o->type != 0x45) {
            d2_ClickedId = d2_Unkf8aacc;
            di_GuiClick = 1;
            c = (D2CTRLX *)o->buf;
            if (c == 0) {
                d2_Unkf8aac4 = -1;
            } else if (c->shown != 0 && c->loaded == 0) {
                d2_Unkf8aac4 = c->id;
            }
            sb_PlaySample(d2_Unk6ed990,
                          o->checkable != 0 ? (o->checked ^= 1, d2_Objs[d2_PointerObj].highlighted = d2_Objs[d2_PointerObj].checked,
                                           "MausclickLinks_s_01")
                                        : d2_Unkf8aacc == 1 ? "MausclickRechts_s_01" : "MausclickLinks_s_01",
                          1, 0x3f, 0x7f, 0, 0);
            found = 0;
            k = d2_PointerObj;
            i = 0;
            do {
                if (found)
                    break;
                j = 0;
                if (RAD(i)->n > 0) {
                    p = RAD(i)->objs;
                    do {
                        if (*p == k) {
                            m = 0;
                            if (RAD(i)->n > 0) {
                                q = RAD(i)->objs;
                                do {
                                    n = *q;
                                    if (n == k) {
                                        d2_SetObjValue(n, 1);
                                        RAD(i)->sel = m;
                                    } else {
                                        d2_SetObjValue(n, 0);
                                    }
                                    m++;
                                    q++;
                                    k = d2_PointerObj;
                                } while (m < RAD(i)->n);
                            }
                            found = 1;
                        }
                        j++;
                        p++;
                    } while (j < RAD(i)->n);
                }
                i += 0x8c;
            } while (i < 0x460);
            if (d2_ClickedId != d2_Unkf8aa9c)
                d2_Unkf8aa9c = d2_ClickedId;
            d2_Unkf8a65c = d2_Unkf8aa80;
        }
        if (k != -1)
            d2_Unkf8a670 = &d2_Objs[k];
    } else {
        d2_ClickedId = -1;
        d2_Unkf8a670 = 0;
        d2_Unkf8a65c = -1;
    }
    my = d2_MouseY;
    mx = d2_MouseX;
    d2_HitClicked = -1;
    d2_HitUnderPointer = -1;
    for (i = 0; i < 0x20; i++) {
        h = &d2_HitRects[i];
        if (!(mx < h->x || h->x + h->w < mx || my < h->y || h->y + h->h < my || h->used == 0)) {
            d2_HitUnderPointer = i;
            if (dm_L.lclick != 0)
                d2_HitClicked = i;
            break;
        }
    }
    win = d2_HitWindow;
    if (win != -1) {
        w = &d2_Wins[win];
        if (dm_L.ldown != 0 && (w->flags & 0x20) != 0 && w->w - 0xc + w->x < mx) {
            v = w->y;
            if (v + 10 < my && my < w->h - 10 + v) {
                n = ((my - v - 0xc) * w->contentHeight) / (w->h - 0x18) + 4;
                w->scrollY = n;
                line = n / d2_FontHeight;
                w->firstline = line;
                *(int *)((char *)w + 0x34) = *(int *)((char *)w + 0x38 + line * 4);
                if (n != w->appliedScrollY)
                    d3io_NoopCallback(win);
            }
        }
    }
}
