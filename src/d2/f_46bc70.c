/* 0x46bc70, sets the widget's label (label, 0x3f bytes) */

void d2_SetLabel(int id, char *s)
{
    strncpy(d2_Objs[id].label, s, 0x3f);
}
