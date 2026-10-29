/* 0x475f40, sets the maximum value (the range) of the input / slider object obj belongs to */

void d2_SetObjMax(int obj, int v)
{
    d2_Inputs[d2_Objs[obj].input].maxval = v;
}
