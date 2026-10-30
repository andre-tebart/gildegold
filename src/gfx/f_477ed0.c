/* 0x477ed0, per frame handling of the input fields and sliders */

int d2_UpdateInput(void)
{
    D2OBJ *o;
    D2INPUT *p;
    D2WIN *w;
    int y, i;

    if (d2_DragActive != 0 && dm_L.lextra == 0 && (dm_L.ldown == 0 || d2_FocusedInput == 0 || (D2CUR->flags & 2) == 0)) {
        d2_DragActive = 0;
        di_MaxY = d2_SavedMaxY;
        di_MinY = d2_SavedMinY;
        d2_EndInputEdit();
    }
    y = dm_L.y;
    if (dm_L.ldown == 0)
        d2_ClickUsed = 0;
    if (d2_FocusedInput == 0 && d2_TextCursorSprite != -1) {
        d2_RemoveObj(d2_TextCursorSprite);
        d2_TextCursorSprite = -1;
    }
    if (d2_PointerObj == -1 || (o = &d2_Objs[d2_PointerObj])->type != 0x41 ||
        d2_PointerObj != (p = (D2INPUT *)o->data)->obj) {
focus:
        if (dm_L.ldown == 0)
            goto click;
        if (d2_FocusedInput != 0) {
            d2_ClickUsed = 1;
            goto click;
        }
    } else {
        if (dm_L.ldown != 0) {
            if (D2CUR != p && d2_ClickUsed == 0) {
                if (D2CUR != 0)
                    d2_Objs[D2CUR->obj].highlighted = 0;
                d2_FocusedInput = p;
                d2_Objs[p->obj].highlighted = 1;
                d2_InputDragOffsetY = di_MaxY - y;
                d2_InputDragValue = p->value;
                d2_InputDragReleased = 0;
                w = d2_Objs[D2CUR->obj].win;
                i = 0;
                if (w != 0) {
                    while (1) {
                        if (i >= w->nobjs)
                            break;
                        if (w->objs[i] == p->obj)
                            d2_FocusObjIndex = i;
                        i++;
                    }
                }
                if ((D2CUR->flags & 0x40) != 0 && D2CUR->maxval == 0 && D2CUR->minval == 0) {
                    if (d2_CountPointerItems() == 0 && D2CUR->value == 0) {
                        D2CUR->value = -1;
                    }
                }
                if (D2CUR->flags & 1) {
                    d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
                    d2_TextCursorY = D2CUR->y;
                    if (d2_TextCursorSprite == -1) {
                        d2_TextCursorSprite = d2_CreatePicObj(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorPic);
                    } else {
                        d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorSprite);
                    }
                } else {
                    if (d2_TextCursorSprite != -1) {
                        d2_RemoveObj(d2_TextCursorSprite);
                        d2_TextCursorSprite = -1;
                    }
                }
            }
            goto focus;
        }
click:
        if (d2_FocusedInput != 0 && d2_PointerObj == D2CUR->obj && (D2CUR->flags & 2) != 0 && dm_L.lclick != 0) {
            if (D2CUR->maxval > D2CUR->value && D2CUR->value != -1 && D2CUR->icon != -1) {
                D2CUR->value += d2_InputStep(D2CUR);
            }
            if (D2CUR->maxval == 0 && D2CUR->minval == 0) {
                if (d2_CountPointerItems() == 0 && D2CUR->value == 0 && (D2CUR->flags & 0x40) != 0) {
                    D2CUR->value = -1;
                }
            }
        }
    }
    if (dm_L.rup != 0) {
        if (d2_FocusedInput == 0)
            goto last;
        if ((D2CUR->flags & 1) != 0) {
            di_ClearEvents(0x10);
        }
        d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = 0;
        d2_ClickUsed = 0;
        if (d2_TextCursorSprite == -1)
            goto last;
        d2_RemoveObj(d2_TextCursorSprite);
        d2_TextCursorSprite = -1;
    }
    if (d2_FocusedInput != 0) {
        if (dm_L.ldown == 0) {
            d2_InputDragReleased = 1;
            dm_L.lclick = 0;
        }
        d2_InputKey();
        d2_DragInput();
        if (d2_FocusedInput != 0 && (D2CUR->flags & 0x10) != 0 && d2_PointerObj == D2CUR->obj && dm_L.lclick != 0) {
            D2CUR->currentTextId = D2CUR->currentTextId + 1;
            if (D2CUR->currentTextId > D2CUR->textCount - 1 + D2CUR->firstTextId)
                D2CUR->currentTextId = D2CUR->firstTextId;
            d2_InputSetString(D2CUR->id, D2CUR->currentTextId);
        }
    }
last:
    d2_SliderInput();
    if (d2_PointerObj != -1 && (d2_Objs[d2_PointerObj].style & 0x40) != 0)
        d2_CopyCaption(d2_Objs[d2_PointerObj].caption);
    return 0;
}
