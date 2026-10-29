/* 0x413640, d3t_SetColor on obj and its subtree; with bit 0 of state set the walk also goes on to obj->next */

void d3t_SetTreeColor(D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char rgb[3];
    D3OBJECT *next;

    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    if (obj) {
        if (!(obj->state & 1)) {
            next = obj->next;
            obj->next = 0;
            d3_ForEachObject(d3_DefaultScene, obj, 0x1ff, d3t_SetColorCB, rgb);
            obj->next = next;
        } else
            d3_ForEachObject(d3_DefaultScene, obj, 0x1ff, d3t_SetColorCB, rgb);
    }
}
