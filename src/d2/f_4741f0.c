/* 0x4741f0, loads the form file x (forms\x.form) as a view: one window per record, the items (pictures, texts,
   inputs, sliders) become objects; the view is moved to (x, y) unless one of them is 0 and kept inside the screen;
   returns the number of the view, or -1
   @deep B3 far: 988 code hunks (735 differing of 712 instructions) and the whole shape is still wrong: our frame is 0x244, the original's 0x248 (Ghidra's lowest local is local_248), so every stack slot shifts by 4 (slots hunks at 474276/4742b2) and all parameter/local offsets differ by 4 - one 4-byte local is missing, but neither Ghidra's pseudocode nor the source shows which (Ghidra has local_230..local_248 that do not map one-to-one). Readability is done (Q1..Q7 clean): named from the data and the ~300 callers d2_OpenForm, d2_FullFormPath, d2_FormsDir, D2VIEW.firstObj/enabled, D2FORM0.bgpic, and the D2FORM2.alphaBits union view; the original's hardcoded view-scan guard 0xfb91e4 is D2_VIEW_END_GUARD (confirmed at 0x474376). Meaning checked against 0x4741f0..0x474b10: the form-file header/version branches, the free-view scan, the window loop and item loop follow Ghidra; no wrong constant/type/offset found beyond the frame size. spell.py leads tried/read: reload at 4741f0 (the original re-reads [esp+0x264] = name; ours keeps it in ebx - same parameter, only the frame offset differs, no local copy to drop), reloads at 4742ce/474329 same class; code hunks at 47423e/474368 have no menu rule; the slots hunks are the frame shift. Needs the deep toolbox (vargen/listing to find the missing local and the allocator cascade). */

/* 0x474b30 in the order of the file: 0x4741f0 first (loads the view), below */

/* the original's hardcoded end guard of the free-view scan in d2_OpenForm (0x474376: cmp ecx,0xfb91e4);
   it does not line up with d2_Views + 0x30 (0xfb6060) - the scan normally stops at the first free view */
#define D2_VIEW_END_GUARD ((unsigned)&d3s_Lights + 0x4)

int d2_OpenForm(int x, int y, char *name)
{
    char buf[0x100] = "";
    char msg[0x100];
    char hdr[5];
    unsigned char fl;
    void *f;
    int n, ver, view, i, k, win, pw, o, pic, idx, parent;
    D2WINF *ww, *pww;
    D2FORMH *r;
    int *pn;

    strcpy(buf, name);
    if (d2_FullFormPath == 0) {
        strcpy(buf, d2_FormsDir);
        strcat(buf, name);
        strcat(buf, ".form");
    }
    f = vfs_Tfopen(buf, "rb", vfs_Root);
    if (f == 0) {
        sprintf(msg, "Konnte die Form\n %s\nnicht laden...", name);
        return -1;
    }
    vfs_fread(hdr, 1, 4, f);
    ver = hdr[3] - '0';
    hdr[4] = 0;
    if (ver == 2) {
        vfs_fread(&n, 4, 1, f);
        vfs_fread(d2_Forms2, 0x101c, n, f);
    } else {
        ver = 0;
        vfs_fseek(f, 0, 0);
        vfs_fread(&n, 4, 1, f);
        vfs_fread(d2_Forms0, 0xed4, n, f);
    }
    vfs_fclose(f);
    view = 1;
    if (d2_Views[1].nwins != 0) {
        pn = &d2_Views[1].nwins;
        do {
            if ((int)pn >= D2_VIEW_END_GUARD)
                return -1;
            pn += 0xab;
            view++;
        } while (*pn != 0);
        if (view >= 0x30)
            return -1;
    }
    d2_Views[view].firstObj = d2_HighestObj - 1;
    for (i = 0; i < n; i++) {
        if (ver == 0) {
            r = &d2_Forms0[i].h;
            parent = d2_Forms0[i].parent;
            if (parent != 0) {
                pw = ((int *)&d2_Views[view])[parent];
                pww = (D2WINF *)&d2_Wins[pw];
            }
            if (parent == 0)
                win = d2_CreateWindow(r->x, r->y, r->w, r->h, r->flags);
            else
                win = d2_CreateChildWindow(r->x - pww->x, r->y - pww->y, r->w, r->h, r->flags, pw);
            d2_Views[view].wins[i] = win;
        } else if (ver == 2) {
            r = &d2_Forms2[i].h;
            if (d2_Forms2[i].parent == 0)
                win = d2_CreateWindow(r->x, r->y, r->w, r->h, r->flags);
            else {
                pw = ((int *)&d2_Views[view])[d2_Forms2[i].parent];
                pww = (D2WINF *)&d2_Wins[pw];
                win = d2_CreateChildWindow(r->x - pww->x, r->y - pww->y, r->w, r->h, r->flags, pw);
            }
            d2_Views[view].wins[i] = win;
            if (d2_Forms2[i].bg[0] != 0)
                d2_SetBackgroundTexture(d2_Forms2[i].bg, d2_LastWindow);
            if ((d2_Wins[d2_LastWindow].flags & 1) == 0 || d2_Forms2[i].alpha != 0.0f)
                ((D2WINF *)&d2_Wins[d2_LastWindow])->alpha = d2_Forms2[i].alphaBits;
            else
                ((D2WINF *)&d2_Wins[d2_LastWindow])->alpha = 0x3f266666;
        }
        ww = (D2WINF *)&d2_Wins[d2_Views[view].wins[i]];
        if (ver == 2) {
            char *fn = d2_Forms2[i].font;

            if (fn[0] == 0)
                fn = "_FONT";
            ww->font = d2_FindPic(fn);
            if (d2_Forms2[i].pic[0] != 0) {
                ww->pic = d2_FindPic(d2_Forms2[i].pic);
                d2_AddRefcount(ww->pic);
            }
            ww->m[0] = d2_Forms2[i].m[0];
            ww->m[1] = d2_Forms2[i].m[1];
            ww->m[3] = d2_Forms2[i].m[3];
            ww->m[2] = d2_Forms2[i].m[2];
            if (ww->flags & 4) {
                idx = d2_FindPic(d2_Forms2[i].frame);
                ww->frame = idx;
                if (idx == -1)
                    ww->frame = d2_WinBorderPic;
            }
        }
        k = 0;
        d2_Objs[ww->parent].buf = (D2CTRL *)&d2_Views[view];
        win = d2_Views[view].wins[i];
        d2_Views[view].nwins++;
        r = ver == 2 ? &d2_Forms2[i].h : &d2_Forms0[i].h;
        for (k = 0; k < r->n; k++) {
            if (r->type[k] < 0x40 && (pic = d2_FindPic(r->name[k])) != -1) {
                o = d2_WinOpenPic(r->ix[k], r->iy[k], pic, win);
                if (r->kind[k] == 0x12) {
                    d2_Objs[o].checkable = 1;
                    d2_Objs[o].interactive = 0;
                }
                if (r->kind[k] == 8) {
                    d2_Objs[o].checkable = 0;
                    d2_Objs[o].interactive = 1;
                }
                d2_Objs[o].buf = (D2CTRL *)&d2_Views[view];
            }
            if (r->type[k] == 0x43) {
                if (strcmp(r->name[k], "") != 0) {
                    idx = txt_FindKey(r->name[k]);
                    if (idx != -1) {
                        o = d2_CreateWinText(r->ix[k], r->iy[k], d2_Strings[idx], win);
                        d2_Objs[o].pic = txt_FindKey(r->name[k]);
                        d2_Objs[o].buf = (D2CTRL *)&d2_Views[view];
                    }
                    continue;
                }
            }
            if (r->type[k] == 0x41) {
                fl = (unsigned char)r->kind[k];
                o = d2_WinOpenInput(r->ix[k], r->iy[k], ((unsigned char *)&r->kind[k])[1],
                                    ((unsigned char *)&r->kind[k])[2], fl, win);
            } else if (r->type[k] == 0x45) {
                fl = (unsigned char)r->kind[k];
                o = d2_WinOpenSlider(r->ix[k], r->iy[k], d2_Forms2[i].slen[k], 0, 100, d2_FindPic(r->name[k]), fl, win);
            } else
                continue;
            d2_Objs[o].buf = (D2CTRL *)&d2_Views[view];
        }
        if (r->key[0] == 0)
            ((int *)ww)[0x90] = -1;
        else {
            idx = txt_FindKey(r->key);
            if (idx != -1)
                strcpy(ww->text, d2_Strings[idx]);
            ((int *)ww)[0x90] = idx;
        }
        if (ver == 0) {
            short fnt;

            ww->m[1] = d2_Forms0[i].m[1];
            ww->m[0] = d2_Forms0[i].m[0];
            ww->m[2] = d2_Forms0[i].m[2];
            fnt = (short)d2_Forms0[i].font;
            ww->m[3] = d2_Forms0[i].m[3];
            if (fnt == 0)
                fnt = d2_FindPic("_FONT");
            ww->font = fnt;
            if (d2_Forms0[i].bgpic > 0)
                ww->pic = d2_Forms0[i].bgpic;
        }
    }
    d2_Views[view].active = 1;
    d2_Views[view].shown = 1;
    d2_Views[view].enabled = 1;
    d2_Views[view].id = view;
    d2_CurrentView = view;
    d2_FullFormPath = 0;
    strcpy(d2_Views[view].pad22c, name);
    ww = (D2WINF *)&d2_Wins[d2_Views[view].wins[0]];
    if ((short)x != 0 && (short)y != 0)
        d2_MoveObj(x, y, ww->parent);
    if (d2_ScreenW < ww->x + ww->w)
        d2_MoveObj(d2_ScreenW - ww->w, ww->y, ww->parent);
    return view;
}
