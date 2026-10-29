/* 0x46c260, sets the width of an object and centres it again */

void d2_SetObjWidth(int obj, short w)
{
    d2_Objs[obj].w = w;
    d2_CenterObj(obj);
}
