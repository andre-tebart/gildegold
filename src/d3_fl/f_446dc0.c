/* 0x446dc0, sets the tile shift (low nibble of map->flags byte +0x1c71), rebuilding the tiles and the light map */

void d3fl_SetShift(D3FLMAP *map, unsigned char shift, char light)
{
    if (map->shift != shift) {
        d3fl_FreeTiles(map);
        map->shift = shift;
        d3fl_Inflate(map);
        if (light)
            d3fl_MakeLight(map);
    }
}
