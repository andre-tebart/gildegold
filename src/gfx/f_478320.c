/* 0x478320, gives the input object obj the focus */

void d2_FocusInput(int obj)
{
    D2INPUT *in;
    D2WIN *w;
    int i;

    in = (D2INPUT *)d2_Objs[obj].data;
    if (d2_FocusedInput != in) {
        if (d2_FocusedInput != 0)
            d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = in;
        d2_Objs[in->obj].highlighted = 1;
        w = d2_Objs[D2CUR->obj].win;
        i = 0;
        if (w != 0) {
            while (1) {
                if (i >= w->nobjs)
                    break;
                if (w->objs[i] == in->obj)
                    d2_FocusObjIndex = i;
                i++;
            }
        }
        if ((D2CUR->flags & 0x40) != 0 && D2CUR->maxval == 0 && D2CUR->minval == 0) {
            if (d2_CountPointerItems() == 0 && D2CUR->value == 0)
                D2CUR->value = -1;
        }
        if ((D2CUR->flags & 2) != 0 && D2CUR->maxval != 0)
            D2CUR->value = 1;
        if ((D2CUR->flags & 1) != 0) {
            d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
            d2_TextCursorY = D2CUR->y;
            if (d2_TextCursorSprite == -1)
                d2_TextCursorSprite = d2_CreatePicObj(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorPic);
            else
                d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - d2_TextCursorOffset, d2_TextCursorSprite);
        } else if (d2_TextCursorSprite != -1) {
            d2_RemoveObj(d2_TextCursorSprite);
            d2_TextCursorSprite = -1;
        }
    }
}
