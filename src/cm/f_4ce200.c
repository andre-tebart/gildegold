/* 0x4ce200, the distance of the character c's model to the cell (x, y) */

float ai_Unk4ce200(char *c, int x, int y)
{
    float v[3];
    char *p;
    float dx, dz;

    d3sm_GetVertex(*(int *)(ai_Unk5fb5b0 + 0xb0), x, y, v);
    v[1] = 0;
    p = *(char **)(*(char **)(c + 0x184) + 0x34);
    dx = v[0] - *(float *)(p + 0x4c);
    dz = v[2] - *(float *)(p + 0x54);
    return (float)sqrt(dz * dz + dx * dx);
}
