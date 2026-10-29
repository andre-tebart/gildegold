/* 0x46c6c0, keyboard / pointer navigation through a menu (arrow keys, enter) */

void d2_MenuNavigate(menu)
int menu;
{
    int count, cur, sel, i, id;
    D2OBJ *o;

    count = d2_Radios[menu].n;
    if (count <= 0)
        return;
    cur = d2_Radios[menu].sel;
    sel = d2_Radios[menu].sel;
    if (d2_PointerMoved == 0) {
        if (d2_LastPointerX != dm_L.x || d2_LastPointerY != dm_L.y) {
            d2_PointerMoved = 1;
            goto check;
        }
    } else {
check:
        if (d2_FocusedInput == 0) {
            for (i = 0; i < count; i++) {
                if (d2_Radios[menu].objs[i] == d2_PointerObj &&
                    ((d2_Radios[menu].flags & 1) == 0 || dm_L.lup != 0))
                    sel = i;
            }
            if (sel == cur)
                d2_PointerMoved = 0;
        }
    }
    if (kb_Last == 0xc8 || (dm_L.wup != 0 && d2_PointerWinVisible == -1)) {
        sel--;
        d2_PointerMoved = 0;
        d2_LastPointerX = dm_L.x;
        d2_LastPointerY = dm_L.y;
        if (sel < 0)
            sel = count - 1;
        sel = sel % count;
        if (dm_L.wup != 0) {
            o = &d2_Objs[d2_Radios[menu].objs[sel]];
            di_SetMousePos((int)(o->w * 0.5f + o->x), (int)(o->h * 0.5f + o->y));
        }
    } else if (kb_Last == 0xd0 || (dm_L.wdown != 0 && d2_PointerWinVisible == -1)) {
        sel++;
        d2_PointerMoved = 0;
        d2_LastPointerX = dm_L.x;
        d2_LastPointerY = dm_L.y;
        if (sel < 0)
            sel = count - 1;
        sel = sel % count;
        if (dm_L.wdown != 0) {
            o = &d2_Objs[d2_Radios[menu].objs[sel]];
            di_SetMousePos((int)(o->w * 0.5f + o->x), (int)(o->h * 0.5f + o->y));
        }
    }
    if (sel != -1) {
        if (sel < 0)
            sel = d2_Radios[menu].n - 1;
        sel = sel % d2_Radios[menu].n;
        if (sel != d2_Radios[menu].sel || d2_PointerMoved == 0)
            d2_RadioSelect(menu, sel);
        if (kb_Last == 0x1c) {
            dm_L.lclick = 1;
            kb_Last = 0;
            id = d2_Radios[menu].objs[sel];
            d2_PointerObj = id;
            o = &d2_Objs[id];
            if (o->type == 9)
                if ((char)o->style & 0x10)
                    d2_ClickedId = 1;
                else
                    d2_ClickedId = 0;
            else
                d2_ClickedId = o->pic;
        }
    } else {
        if (d2_Radios[menu].sel != -1)
            d2_RadioSelect(menu, -1);
    }
}
