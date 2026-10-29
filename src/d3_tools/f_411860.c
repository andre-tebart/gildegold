/* 0x411860, d3t_PlaceObject with the target position and direction given as vectors */

int d3t_PlaceObjectV(D3OBJECT *camera, D3VECTOR *pos, D3VECTOR *dir, int unk, char flags)
{
    return d3t_PlaceObject(camera, pos->x, pos->y, pos->z, dir->x, dir->y, dir->z, unk, flags);
}
