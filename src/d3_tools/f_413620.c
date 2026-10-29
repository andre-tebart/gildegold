/* 0x413620, object walk callback: d3t_SetColor with an r/g/b triple */

static char d3t_SetColorCB(D3OBJECT *obj, void *rgb)
{
    d3t_SetColor(obj, ((unsigned char *)rgb)[0], ((unsigned char *)rgb)[1], ((unsigned char *)rgb)[2]);
    return 1;
}
