/* 0x4437f0, culls the tiles against the view and picks their detail levels; returns 1 if any level changed @deep B: read the whole 112-line asmdiff against 0x4437f0..0x4439e0 and diffclass: what is left is the local layout - the original initialises its locals in another order and keeps map in eax where ours uses edi, computes the first loop's tile addresses as (y*8+x)*25 chains of lea where the original walks a tile pointer (add esi,0x64), and assigns x/y/i/n/sum to other stack slots; the tail is the misaligned disassembly of the shorter function. Meaning fixes applied while reading: the unsigned guards are spelled > 0 (t->lodLevel, the TileLod result, k, y) which reproduces the original's jbe; no other statement/constant/callee differs. Needs vargen/try.py */

char d3fl_UpdateTiles(D3FLMAP *map)
{
    D3FLTILE *t;
    D3BOXVERT *box;
    unsigned int x, y, k;
    int i, n = 0;
    char changed = 0;
    float sum = 0.0f;
    D3VECTOR4 c;
    unsigned char lod;

    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++) {
            box = d3fl_TileBox(map, &map->tiles[y][x], x, y);
            if ((map->tiles[y][x].clip = d3_ClipBox(box, &map->tiles[y][x].zmin, &map->tiles[y][x].zmax)) & 0x40)
                map->tiles[y][x].lodLevel = 0;
            else {
                map->tiles[y][x].lodLevel = 1;
                map->tiles[y][x].zmin += 16.0f;
                map->tiles[y][x].zmax += 16.0f;
                c.x = box[0].x;
                c.y = box[0].y;
                c.z = box[0].z;
                for (i = 1; i < 8; i++) {
                    c.x += box[i].x;
                    c.y += box[i].y;
                    c.z += box[i].z;
                }
                n++;
                c.x *= 0.125f;
                c.y *= 0.125f;
                c.z *= 0.125f;
                map->tiles[y][x].center.x = c.x;
                map->tiles[y][x].center.y = c.y;
                map->tiles[y][x].center.z = c.z;
                sum += map->tiles[y][x].centerDist = (float)sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
            }
        }
    for (y = 0; y < 8; y++)
        for (k = 0; k < 8; k++) {
            t = &map->tiles[y][k];
            if (t->lodLevel > 0 && (t->lodLevel = lod = d3fl_TileLod(map, t, sum, n)) > 0) {
                if (k > 0 && (lod == 4 && t[-1].lodLevel == 1 || lod == 1 && t[-1].lodLevel == 4))
                    t->lodLevel = 2;
                if (y > 0 && (t->lodLevel == 4 && t[-8].lodLevel == 1 || t->lodLevel == 1 && t[-8].lodLevel == 4))
                    t->lodLevel = 2;
                t->lodLevel = max(1 << map->shift, t->lodLevel);
            }
            if (t->lodLevel != (unsigned char)t->lod)
                changed = 1;
        }
    return changed;
}
