/* 0x4734d0, sets the text of a text object */

void d2_SetObjText(int obj, char *text)
{

    strcpy((char *)d2_Objs[obj].input, text);
    if (d2_Objs[obj].centred == 0 && d2_Objs[obj].rightAligned == 0)
        d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font);
}
