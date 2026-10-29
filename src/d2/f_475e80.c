/* 0x475e80, scales a picture object: a > 1 times, a < -1 times smaller */

void d2_ScaleObject(int obj, int a)
{
    int w, h, n;

    if (obj != -1) {
        if (d2_GetObjPicSize(obj, &w, &h) == 0) {
            d2_GetObjPicSize(obj, &w, &h);
            return;
        }
        d2_Objs[obj].picScale = a;
        if (a > 1) {
            d2_Objs[obj].w = (short)w * (short)a;
            d2_Objs[obj].h = (short)h * (short)a;
        }
        if (a < -1) {
            n = abs(a);
            d2_Objs[obj].w = (short)(w / n);
            d2_Objs[obj].h = (short)(h / n);
        }
    }
}
