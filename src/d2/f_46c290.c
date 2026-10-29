/* 0x46c290, opens an object with w x h 16-bit pixels of its own, copied from src if given */

int d2_OpenRaw(int x, int y, int w, int h, void *src)
{
    int obj;
    int n;

    obj = d2_NewObject();
    d2_Objs[obj].type = 0x47;
    d2_Objs[obj].x = x;
    d2_Objs[obj].y = y;
    d2_Objs[obj].h = h;
    d2_Objs[obj].w = w;
    d2_Objs[obj].pic = -1;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    n = w * h * 2;
    d2_Objs[obj].u.raw = mem_Alloc(n, "d2:raw");
    if (src)
        memcpy(d2_Objs[obj].u.raw, src, n);
    return obj;
}
