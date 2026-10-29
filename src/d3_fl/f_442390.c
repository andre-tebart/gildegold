/* 0x442390, creates a terrain map from a description: height, water and texture maps either from memory or from
   "boden/<name>.bmp", "boden/<name>_w.bmp" and "boden/<texname>.bmp"
   @deep B: 6 code hunks, each the same operations in another spelling, read against the original disassembly (0x442390..0x442ac0), Ghidra output and the source: (1) 0x442582 idxMask/mask: same size*size-1|size-1, the original reuses eax and lea where ours reloads map->size for mask and uses dec; (2) 0x4425c4 water mem_Alloc+D3FL_COPY: same size*size computation and desc->water copy, scheduled on the other side of the push/call (ecx vs edx); (3) 0x442944 transform: same map->n=size/8 division and (float)-size*scale*0.5f, integer division and FPU multiplies interleaved differently, map->size loaded twice vs once+copy; (4) 0x44299b and (5) 0x4429d3: the same scale/hscale field copies, mov vs fld/fstp on the opposite statement each; (6) 0x442a13 typenames loop: mov al vs mov cl, a register rename only; tail = fill nops. No missing statement, constant, callee, condition or field; readability fixed (Q2 idxMask) */

D3FLMAP *d3fl_LoadMap(D3FLDESC *desc)
{
    char path[256];
    char msg[256];
    char dir[256];
    D3FLMAP *map;
    char *p;
    int w, h;
    D3MATRIX *m;
    int i, n, mn, mx, v;

    map = mem_Alloc(sizeof(D3FLMAP), "d3_fl:Floor");
    strcpy(map->name, desc->name);
    strcpy(map->texname, desc->texname);
    map->shift = d3fl_MapShift;
    if (desc->height && (n = desc->size) > 0) {
        map->size = n;
        map->height = mem_Alloc(n * n, "d3_fl:Height");
        D3FL_COPY(map->height, desc->height, map->size * map->size);
    } else {
        vfs_DirPath(d3_AnimPath, vfs_Root, path);
        strcat(path, "boden/");
        strcpy(dir, path);
        strcat(path, desc->name);
        strcat(path, ".bmp");
        if (!bmp_GetInfo(path, &w, &h, 0) || w != h)
            sprintf(msg, "Could not open HeightMap-File %s or HeightMap has wrong size!!!", path);
        else {
            map->size = w;
            map->height = mem_Alloc(w * w, "d3_fl:Height");
            bmp_Load(path, map->height, 0, 5);
        }
    }
    map->idxMask = map->size * map->size - 1 | map->size - 1;
    map->mask = map->size - 1;
    if (desc->nregions > 0 && desc->regions) {
        if (desc->water && desc->size > 0) {
            map->water = mem_Alloc(map->size * map->size, "d3_fl:Water(height)");
            D3FL_COPY(map->water, desc->water, map->size * map->size);
        } else {
            strcpy(path, dir);
            strcat(path, desc->name);
            strcat(path, "_w.bmp");
            if (!bmp_GetInfo(path, &w, &h, 0) || w != h || w != map->size)
                sprintf(msg, "Could not open WaterMap-File %s or WaterMap has wrong size!!!", path);
            else {
                map->water = mem_Alloc(map->size * map->size, "d3_fl:Water(height)");
                bmp_Load(path, map->water, 0, 5);
            }
        }
        map->nregions = desc->nregions;
        map->regions = mem_Alloc(map->nregions * sizeof(D3FLREGION), "d3_fl:Water(regions)");
        D3FL_COPY(map->regions, desc->regions, map->nregions * sizeof(D3FLREGION));
    }
    if (desc->types && desc->size > 0) {
        map->types = mem_Alloc(map->size * map->size, "d3_fl:Texture");
        D3FL_COPY(map->types, desc->types, map->size * map->size);
    } else {
        strcpy(path, dir);
        strcat(path, desc->texname);
        strcat(path, ".bmp");
        if (!bmp_GetInfo(path, &w, &h, 0) || w != h || w != map->size)
            sprintf(msg, "Could not open Texture-Information-File %s or Texture-Information has wrong size!!!", path);
        else {
            map->types = mem_Alloc(map->size * map->size, "d3_fl:Texture");
            bmp_Load(path, map->types, 0, 5);
        }
    }
    if (desc->lightofs && desc->size > 0) {
        map->lightofs = mem_Alloc(map->size * map->size * 4, "d3_fl:LightOffset");
        D3FL_COPY(map->lightofs, desc->lightofs, map->size * map->size * 4);
    }
    if (map->types && map->height) {
        map->n = map->size / 8;
        m = (D3MATRIX *)&map->orgx;
        m->_11 = (float)-map->size * desc->scale * 0.5f;
        m->_12 = desc->hscale * -64.0f;
        m->_13 = (float)map->size * desc->scale * 0.5f;
        m->_21 = desc->scale;
        m->_22 = 0.0f;
        m->_23 = 0.0f;
        m->_31 = 0.0f;
        m->_32 = 0.0f;
        m->_33 = -desc->scale;
        m->_41 = 0.0f;
        m->_42 = desc->hscale;
        m->_43 = 0.0f;
        map->lodDist2 = 20.0f;
        map->lodDist4 = 40.0f;
        map->nolight = 0;
        map->lod = 0;
        for (i = 0; i < 8; i++) {
            map->typenames[i][0] = d3fl_EmptyName;
            p = desc->typenames[i];
            if (p && *p)
                d3fl_LoadTexture(map, p, -1);
        }
        n = map->size * map->size;
        mn = 0xff;
        mx = 0;
        for (i = 0; i < n; i++) {
            v = map->types[i];
            mx = max(mx, v);
            mn = min(mn, v);
        }
        for (i = 0; i < map->size * map->size; i++)
            map->types[i] -= mn;
        d3fl_InitMap(map);
        return map;
    }
    d3fl_FreeMap(map);
    return 0;
}
