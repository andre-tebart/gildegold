/* 0x446910, water region flag of the map point x, y, or -1 */

int map_Region(D3FLMAP *map, int x, int y)
{
    D3FLWREG *r;

    if (x > 0 && y > 0 && x < map->size && y < map->size)
        for (r = map->tiles[y / map->n][x / map->n].wregions; r && r->y >= 0; r++)
            if (y == r->y && x >= r->x0 && x <= r->x1)
                return (unsigned char)r->flag;
    return -1;
}
