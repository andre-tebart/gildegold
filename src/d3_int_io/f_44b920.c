/* 0x44b920, reads the floor block and loads the map */

D3FLMAP *d3io_ReadFloor(void *f, unsigned int version)
{
    D3FLMAP *map = 0;
    D3FLDESC desc;
    char buf[0x100];
    D3TEXHANDLE *tex;
    D3VECTOR4 pos;
/* */
    int i;

    if (version >= 0x3a6c0009) {
        D3_ZERO(&desc, sizeof desc);
        bio_rd_s(f, desc.name);
        if (desc.name[0] && version >= 0x3a6c00ad) {
            bio_rd_i(f, &desc.size);
            bio_rd_array_quick(f, desc.size, (void **)&desc.height);
            if (version >= 0x3a6c00bb)
                bio_rd_array_quick(f, desc.size * 4, &desc.lightofs);
        }
        if (version >= 0x3a6c00aa) {
            char c;
            bio_rd_c(f, &c);
            if (c) {
                bio_rd_i(f, &desc.nregions);
                if (desc.nregions > 0) {
                    if (version >= 0x3a6c00ad)
                        bio_rd_array_quick(f, desc.size, (void **)&desc.water);
                    desc.regions = mem_Alloc(desc.nregions * sizeof(D3FLREGION), "d3_int_io:FloorWaterRegions");
                    for (i = 0; i < desc.nregions; i++) {
                        char name[0x40];
                        char c1, c2, c3;
                        D3IOFLAGS fl;
                        tex = 0;
                        bio_rd_s(f, name);
                        bio_rd_c(f, &c1);
                        bio_rd_c(f, &c2);
                        bio_rd_c(f, &c3);
                        if (name[0]) {
                            fl.all = 0;
                            fl.s.b0 = 0x7f;
                            tex = D3IO_TEXLOAD(name, fl, (c2 & 0xf) << 2 | c3 & 1, 0);
                            if (tex)
                                tex_Reload(tex, 0);
                        }
                        fl.all = 0;
                        bio_rd_c(f, &c);
                        fl.s.b0 = c;
                        bio_rd_c(f, &c);
                        fl.s.b1 = c;
                        bio_rd_c(f, &c);
                        fl.s.c0 = c;
                        bio_rd_c(f, &c);
                        fl.s.c1 = c;
                        desc.regions[i].tex2 = (D3FLTEX *)tex;
                        desc.regions[i].tex = (D3FLTEX *)tex;
                        desc.regions[i].flags = fl.f;
                        if (version >= 0x3a6c00b6)
                            bio_rd_i(f, (int *)&desc.regions[i].f14);
                        else
                            desc.regions[i].f14 = 63.0f;
                        bio_rd_i(f, (int *)&desc.regions[i].scrollSpeedU);
                        bio_rd_i(f, (int *)&desc.regions[i].scrollSpeedV);
                        bio_rd_v4(f, (int *)desc.regions[i].speed);
                        if (version < 0x3a6c00ac) {
                            desc.regions[i].speed[0] *= 1.0f / 3.0f;
                            desc.regions[i].speed[1] *= 1.0f / 3.0f;
                            desc.regions[i].speed[2] *= 1.0f / 3.0f;
                            desc.regions[i].speed[3] *= 1.0f / 3.0f;
                        }
                        bio_rd_v4(f, (int *)desc.regions[i].amp);
                        if (version >= 0x3a6c00b9) {
                            int water;
                            bio_rd_i(f, &water);
                            desc.regions[i].water = water;
                        } else
                            desc.regions[i].water = 0;
                    }
                }
            }
        }
        bio_rd_s(f, desc.texname);
        if (version >= 0x3a6c00ad && desc.texname[0])
            bio_rd_array_quick(f, desc.size, (void **)&desc.types);
        if (version <= 0x3a6c000f)
            for (i = 0; i < 5; i++)
                bio_rd_s(f, buf);
        if (version >= 0x3a6c00b0) {
            for (i = 0; i < 8; i++)
                bio_rd_s(f, desc.typenames[i]);
        } else {
            for (i = 0; i < 6; i++)
                bio_rd_s(f, desc.typenames[i]);
        }
        bio_rd_i(f, (int *)&desc.scale);
        bio_rd_i(f, (int *)&desc.hscale);
        if (version <= 0x3a6c000f)
            for (i = 0; i < 0x100; i++)
                bio_rd_s(f, buf);
        map = d3fl_LoadMap(&desc);
        if (version >= 0x3a6c000e) {
            bio_rd_v3(f, (int *)&pos);
            if (map) {
                map->orgx = pos.x;
                map->orgy = pos.y;
                map->orgz = pos.z;
            }
        }
        if (desc.regions) {
            mem_Free(desc.regions);
            desc.regions = 0;
        }
        if (desc.types) {
            mem_Free(desc.types);
            desc.types = 0;
        }
        if (desc.water) {
            mem_Free(desc.water);
            desc.water = 0;
        }
        if (desc.height) {
            mem_Free(desc.height);
            desc.height = 0;
        }
        if (desc.lightofs) {
            mem_Free(desc.lightofs);
            desc.lightofs = 0;
        }
    } else
        err_Message("Incompatible Floor-Versions... Sorry!");
    return map;
}
