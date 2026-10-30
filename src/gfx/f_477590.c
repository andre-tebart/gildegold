/* 0x477590, keyboard input of the input field being edited */

#define FOCUS_SET     d2_Objs[D2CUR->obj].highlighted = 1;     d2_InputDragOffsetY = di_MaxY - y;     d2_InputDragValue = D2CUR->value;     d2_InputDragReleased = 0;

void d2_InputKey(void)
{
    int y = dm_L.y;
    int len, max, i;
    D2WIN *w;

    if (kb_Last == 0)
        return;
    switch (kb_Last) {
    case 0xe:
        len = strlen(D2CUR->text);
        if (len > 0)
            D2CUR->text[len - 1] = 0;
        kb_Last = 0;
        break;
    case 0x1c:
        d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = 0;
        d2_ClickUsed = 0;
        if (d2_TextCursorSprite != -1) {
            d2_RemoveObj(d2_TextCursorSprite);
            d2_TextCursorSprite = -1;
        }
        break;
    case 1:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x57:
    case 0x58:
        break;
    case 0xf:
        w = d2_Objs[D2CUR->obj].win;
        if (kb_Down[0x36] == 0 && kb_Down[0x2a] == 0) {
            if (d2_FocusObjIndex >= w->nobjs - 1)
                d2_FocusObjIndex = -1;
            for (i = d2_FocusObjIndex + 1; i < w->nobjs; i++) {
                if (d2_Objs[w->objs[i]].type == 0x41 && d2_Objs[w->objs[i]].disabled == 0) {
                    d2_FocusObjIndex = i;
                    d2_Objs[D2CUR->obj].highlighted = 0;
                    d2_FocusedInput = (D2INPUT *)d2_Objs[w->objs[i]].data;
                    FOCUS_SET
                    break;
                }
            }
        } else {
            if (d2_FocusObjIndex == 0)
                d2_FocusObjIndex = w->nobjs;
            for (i = d2_FocusObjIndex - 1; i >= 0; i--) {
                if (d2_Objs[w->objs[i]].type == 0x41 && d2_Objs[w->objs[i]].disabled == 0) {
                    d2_FocusObjIndex = i;
                    d2_Objs[D2CUR->obj].highlighted = 0;
                    d2_FocusedInput = (D2INPUT *)d2_Objs[w->objs[i]].data;
                    FOCUS_SET
                    break;
                }
            }
        }
        if (d2_TextCursorSprite != -1) {
            d2_RemoveObj(d2_TextCursorSprite);
            d2_TextCursorSprite = -1;
        }
        if ((D2CUR->flags & 1) != 0) {
            d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
            d2_TextCursorY = D2CUR->y;
            if (d2_TextCursorSprite == -1)
                d2_TextCursorSprite = d2_CreatePicObj(d2_TextCursorX, d2_TextCursorY - 7, d2_ExtraSymbolsPic + 8);
            else
                d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - 7, d2_TextCursorSprite);
        }
        kb_Last = 0;
        break;
    default:
        if (D2CUR->flags & 2) {
            if (kb_Last < 2 || kb_Last > 0xb || kb_Down[0x36] != 0 || kb_Down[0x2a] != 0)
                break;
            _itoa(D2CUR->value, D2CUR->text, 10);
        }
        len = strlen(D2CUR->text);
        if (D2CUR->flags & 1)
            max = D2CUR->maxval;
        else
            max = D2CUR->ndigits;
        if (len < max && (D2CUR->maxw == 0 || d2_TextCursorX <= D2CUR->x + D2CUR->maxw)) {
            if (kb_Down[0x36] == 0 && kb_Down[0x2a] == 0)
                D2CUR->text[len] = kb_FindKey(0, 0, 0, kb_Last);
            else
                D2CUR->text[len] = kb_FindKey(1, 0, 0, kb_Last);
            D2CUR->text[len + 1] = 0;
        }
        kb_Last = 0;
        break;
    }
    if (d2_FocusedInput != 0) {
        if (D2CUR->flags & 2)
            D2CUR->value = atoi(D2CUR->text);
        d2_TextCursorX = d2_TextWidth(D2CUR->text, D2CUR->font) + D2CUR->x;
        if (d2_TextCursorSprite != -1)
            d2_MoveObjI(d2_TextCursorX, d2_TextCursorY - 7, d2_TextCursorSprite);
    }
}
