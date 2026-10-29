/* 0x473b30, replaces the string of an input object inside the window text */

void d2_ReplaceInputText(int obj, char *text)
{
    D2OBJ *o = &d2_Objs[obj];
    D2WIN *w = o->win;
    char *buf;
    int len, old;

    if (w && (buf = (char *)o->u.s.value) != 0) {
        old = o->insertedLen;
        if (old)
            memmove(buf, buf + old, strlen(w->text) + (w->text - buf) + 1);
        len = strlen(text);
        o->insertedLen = len;
        memmove((char *)o->u.s.value + len, (char *)o->u.s.value,
                strlen(w->text) + (w->text - (char *)o->u.s.value) + 1);
        memcpy((char *)o->u.s.value, text, len);
        o->w = d2_TextWidth(text, o->font);
        strcpy(&o->u.text[4], text);
    }
}
