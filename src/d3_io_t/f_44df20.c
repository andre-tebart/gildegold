/* 0x44df20, radius of the mesh behind an object info (the first argument is not used) */

float d3iot_GetRadius(int unused, D3OBJINFO *info)
{
    if (info && info->mesh)
        return ((D3BGF *)info->mesh)->radius;
    return 0.0f;
}
