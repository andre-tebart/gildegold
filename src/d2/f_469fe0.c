/* 0x469fe0, puts string number str (d2_Strings) into the input's text buffer, in front of the window text */

void d2_InputSetString(int in, int str)
{
    D2INPUT *p = &d2_Inputs[in];
    D2WIN *w = d2_Objs[p->obj].win;

    if (w && p->buf) {
        if (p->buflen)
            memmove(p->buf, p->buf + p->buflen, strlen(w->text) + (w->text - p->buf) + 1);
        p->buflen = strlen(txt_Ptrs[str]);
        memmove(p->buf + p->buflen, p->buf, strlen(w->text) + (w->text - p->buf) + 1);
        memcpy(p->buf, txt_Ptrs[str], p->buflen);
        p->w = d2_TextWidth(txt_Ptrs[str], p->font);
        d2_Objs[p->obj].w = p->w;
    }
}
