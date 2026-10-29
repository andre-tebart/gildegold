/* 0x449d80, d3_ForEachObject callback: scales the intensity of a light */

char d3_ScaleLightCB(D3OBJECT *obj, float *scale)
{
    D3IF_LIGHT(obj)->power *= *scale;
    return 1;
}
