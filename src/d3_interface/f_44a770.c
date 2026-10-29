/* 0x44a770, writes the terrain map of the scene */

void d3_WriteMap(VFSFILE *f, D3FLMAP *map)
{
    int i;

    bio_wr_s(f, map->name);
    if (map->name[0]) {
        bio_wr_i(f, map->size);
        bio_wr_array(f, map->size, map->size, map->height);
        bio_wr_array(f, map->size << 2, map->size, map->lightofs);
        bio_wr_c(f, map->water != 0);
        if (map->water) {
            bio_wr_i(f, map->nregions);
            if (map->nregions > 0)
                bio_wr_array(f, map->size, map->size, map->water);
            for (i = 0; i < map->nregions; i++) {
                if (map->regions[i].tex) {
                    bio_wr_s(f, map->regions[i].tex->name);
                    bio_wr_c(f, 0);
                    bio_wr_c(f, ((unsigned char *)map->regions[i].tex)[0x72] & 0xf);
                    bio_wr_c(f, map->regions[i].tex->state & 1);
                } else {
                    bio_wr_s(f, &d3fl_EmptyName);
                    bio_wr_c(f, 0);
                    bio_wr_c(f, 0);
                    bio_wr_c(f, 0);
                }
                bio_wr_c(f, map->regions[i].flags.b[0]);
                bio_wr_c(f, map->regions[i].flags.b[1]);
                bio_wr_c(f, map->regions[i].flags.b[2] & 1);
                bio_wr_c(f, map->regions[i].flags.b[2] >> 1 & 1);
                bio_wr_i(f, ((D3IFREGION *)map->regions)[i].f14.bits);
                bio_wr_i(f, ((D3IFREGION *)map->regions)[i].scrollSpeedU.bits);
                bio_wr_i(f, ((D3IFREGION *)map->regions)[i].scrollSpeedV.bits);
                bio_wr_v4(f, (int *)map->regions[i].speed);
                bio_wr_v4(f, (int *)map->regions[i].amp);
                bio_wr_i(f, map->regions[i].water);
            }
        }
    } else
        bio_wr_c(f, 0);
    bio_wr_s(f, map->texname);
    if (map->texname[0])
        bio_wr_array(f, map->size, map->size, map->types);
    for (i = 0; i < 8; i++)
        bio_wr_s(f, map->typenames[i]);
    bio_wr_i(f, ((D3IFMAP *)map)->scale.bits);
    bio_wr_i(f, ((D3IFMAP *)map)->hscale.bits);
    bio_wr_v3(f, (int *)&map->orgx);
}
