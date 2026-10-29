/* 0x442ac0, the bounding box of tile (tx, ty) in world space (bottom from t->hmin, top from the map's tile) */

#define D3FL_CORNER(i, X0, Z0, H)                                    \
    p.x = d3fl_AxisX.x * (X0) + d3fl_Origin.x;                       \
    p.y = d3fl_AxisX.y * (X0) + d3fl_Origin.y;                       \
    p.z = d3fl_AxisX.z * (X0) + d3fl_Origin.z;                       \
    q.x = d3fl_AxisZ.x * (Z0) + p.x;                                 \
    q.y = d3fl_AxisZ.y * (Z0) + p.y;                                 \
    q.z = d3fl_AxisZ.z * (Z0) + p.z;                                 \
    d3fl_Box[i].x = d3fl_AxisY.x * (H) + q.x;                        \
    d3fl_Box[i].y = d3fl_AxisY.y * (H) + q.y;                        \
    d3fl_Box[i].z = d3fl_AxisY.z * (H) + q.z;

D3BOXVERT *d3fl_TileBox(D3FLMAP *map, D3FLTILE *t, int tx, int ty)
{
    D3VECTOR4 p, q;
    float x0, x1, z0, z1, h;

    x0 = (float)(map->n * tx);
    x1 = (float)((tx + 1) * map->n);
    z0 = (float)(map->n * ty);
    z1 = (float)((ty + 1) * map->n);
    h = t->hmin;
    D3FL_CORNER(0, x0, z0, h)
    D3FL_CORNER(1, x1, z0, h)
    D3FL_CORNER(2, x1, z1, h)
    D3FL_CORNER(3, x0, z1, h)
    h = map->tiles[ty][tx].hmax;
    D3FL_CORNER(4, x0, z0, h)
    D3FL_CORNER(5, x1, z0, h)
    D3FL_CORNER(6, x1, z1, h)
    D3FL_CORNER(7, x0, z1, h)
    return d3fl_Box;
}
