/* 0x474e80, draws the windows of a view onto its image and copies the image onto the textures @deep B3 far draft, readability now clean (Q1-Q7: this session named d2_NoBackgroundRestore and D2VIEW.viewX/viewY); 568/592 instructions, 114 differing (code 114, regs 36, order 18, fill 8). Every hunk read: 4751e5 (clip-rect loads reordered, original edx/eax/ecx vs ours eax/ecx/edx in the 5/8 caption block), 47529e / 47542a / 4754c8 (case 0x43 text-call h/w load order and the two caption tails), 4753cb (no menu rule: ours hoists the test, original branches first), 475403 (reload: original re-reads [esp+0x24] into esi, ours keeps it). spell.py menu read and its prescriptions (dup-tail x4, reload) are the same statement-order / local-form edits the d2m note tried with no effect (18 permutations of the x/w/y statement order, locals for font/width, reuse of x/y, 5 tw*3 spellings, 3 local sizes for 0x43, 2 declaration orders, int mode; reverted). Remaining: whole-function register allocation and scheduling -> deep toolbox (listing/slotmap/vargen) */

void d2_RedrawView(int view)
{
    D2VIEW *v = &d2_Views[view];
    D2OBJ *o;
    D2WIN *w;
    D2TEXX *t;
    void *surf;
    int i, j, x, y, tx, ty, tw, th;
    unsigned int mode;

    if (v->active != 0 && v->loaded != 0) {
        d2_RestoreShapes((D2BUF *)v);
        surf = (void *)v->viewSurf;
        d2_LockSurface(surf);
        for (i = 0; i < v->nwins; i++) {
            int win = v->wins[i];

            w = &d2_Wins[win];
            d2_DrawWindow(win, surf);
            if (d2_Objs[w->parent].win == 0)
                d2_AddShape(w->x - v->viewX, w->y - v->viewY, w->w, w->h, (D2BUF *)v);
            for (j = 0; j < w->nobjs; j++) {
                o = &d2_Objs[w->objs[j]];
                x = o->x - v->viewX;
                y = o->y - v->viewY;
                if (o->used != 0) {
                    d2_SetClipRect(o->clipx0, o->clipy0, o->clipx1, o->clipy1);
                    switch (o->type) {
                    case 1:
                        d2_DrawShape(x, y, surf, o->data, 0);
                        if (o->highlighted != 0)
                            d2_DrawShapeMode3(x, y, surf, o->data, 0);
                        if (o->win == 0)
                            d2_AddShape(x, y, d2_ShpW, d2_ShpH, (D2BUF *)v);
                        break;
                    case 0x11:
                        if (o->drawFrames > 0 && d2_NoBackgroundRestore != 1)
                            o->drawFrames--;
                        break;
                    case 4:
                        d2_DrawAnim(o->input, surf);
                        if (o->win == 0)
                            d2_AddShape(x, y, d2_ShpW, d2_ShpH, (D2BUF *)v);
                        break;
                    case 5:
                    case 8:
                        if (o->style & 2) {
                            if (&d2_Objs[d2_PointerObj] == o || o->checked != 0) {
                                if (o->style & 1)
                                    o->highlighted = 1;
                            } else if (o->style & 1)
                                o->highlighted = 0;
                        }
                        if (o->textShadow != 0)
                            d2_DrawShapeMode2(x + 8, y + 8, surf, o->data, (unsigned char)o->input);
                        d2_DrawShape(x, y, surf, o->data, (unsigned char)o->input);
                        if (o->highlighted != 0)
                            d2_DrawShapeMode3(x, y, surf, o->data, (char)o->input);
                        if (o->win == 0) {
                            if (o->textShadow != 0)
                                d2_AddShape(x, y, d2_ShpW + 8, d2_ShpH + 8, (D2BUF *)v);
                            else
                                d2_AddShape(x, y, d2_ShpW, d2_ShpH, (D2BUF *)v);
                        }
                        if (o->u.text[0] != 0) {
                            int ex, ey;

                            if (o->font != 0)
                                d2_SetFont(o->font);
                            mode = 0;
                            if (o->highlighted)
                                mode = 2;
                            else if (o->greyed)
                                mode = 1;
                            if (o->centred != 0)
                                mode |= 8;
                            if (o->rightAligned != 0)
                                mode |= 0x10;
                            d2_GetObjPicSize(o->id, &tw, &th);
                            ex = o->captionX;
                            tx = o->w;
                            if (ex == -1) {
                                ex = o->x + 6 + tw;
                                tx = tx + tw * 3;
                            }
                            ey = o->captionY;
                            if (ey == -1)
                                ey = o->y - 1;
                            d2_DrawTextBox(ex, ey, tx, o->h, surf, o->u.text, mode);
                        }
                        break;
                    case 0x41:
                        d2_DrawInput(o->input, surf);
                        break;
                    case 0x43:
                        if (o->drawFrames > 0) {
                            int font;

                            d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                            font = d2_CurrentFont;
                            d2_SetFont(o->font);
                            mode = 0;
                            if (o->highlighted)
                                mode = 2;
                            else if (o->greyed)
                                mode = 1;
                            if (o->centred != 0)
                                mode |= 8;
                            d2_DrawTextBox(o->x, o->y, o->w, o->h, surf, (char *)o->input, mode);
                            d2_SetFont(font);
                        }
                        break;
                    case 0x45:
                        d2_SetClipRect(o->clipx0, o->clipy0, o->clipx1, o->clipy1);
                        if (o->win != 0)
                            d2_SetSurfaceClip(o->clipx0, o->clipy0, o->win->w, o->win->h, surf);
                        d2_DrawSlider(o->id, (D2SURF *)surf);
                        if (o->win == 0)
                            d2_AddShape(o->x, o->y - 4, o->w, o->h + 8, (D2BUF *)v);
                        d2_SetSurfaceClip(0, 0, ((D2SURF *)surf)->w, ((D2SURF *)surf)->h, surf);
                        break;
                    }
                }
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
            }
        }
        d2_UnlockSurface(surf);
        t = 0;
        if (v->tex1[0] != 0) {
            t = (D2TEXX *)tex_Find(v->tex1);
            if (t != 0)
                d2_BlitPartToDD(0, 0, t->size, t->size, (void *)v->viewSurf, 0, 0, t->surf);
        } else if (v->tex2[0] == 0)
            return;
        if (v->tex2[0] != 0) {
            D2TEXX *t2 = (D2TEXX *)tex_Find(v->tex2);

            if (t2 != 0)
                d2_BlitPartToDD(0, t->size, t2->size, ((D2SURF *)v->viewSurf)->h - t->size, (void *)v->viewSurf, 0, 0,
                             t2->surf);
        }
    }
}
