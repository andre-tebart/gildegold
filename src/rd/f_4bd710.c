/* 0x4bd710, the town map cell x / z of v (local to obj) */

void rd_WorldToCell(float *x, float *z, float *v, char *obj, G15ROAD *r)
{
    float w[4];

    d3_LocalToWorld(obj, v, w);
    *x = (w[0] - r->map->x0) / r->map->cw;
    *z = (w[2] - r->map->z0) / r->map->ch;
}
