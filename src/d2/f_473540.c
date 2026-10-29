/* 0x473540, sets the caption of a button and its width */

void d2_SetCaption(int obj, char *text)
{
    D2OBJ *o = &d2_Objs[obj];
    void *pic;
    D2FRAME *f0, *f1;
    int a, b;

    if (d2_Objs[obj].type == 9) {
        pic = o->data;
        f0 = d2_GetFrame(pic, 0);
        f1 = d2_GetFrame(pic, 1);
        a = d2_TextWidth(text, d2_DefaultFont) + f1->w + 4 + f0->w;
        strncpy(o->u.text, text, 0x3f);
        d2_Objs[obj].w = a;
        return;
    }
    if (d2_GetObjPicSize(obj, &a, &b) == 0)
        a = 0x18;
    strncpy(d2_Objs[obj].u.text, text, 0x3f);
    if (d2_Objs[obj].centred == 0) {
        d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font) + 6 + a;
        return;
    }
    d2_Objs[obj].w = a;
}
