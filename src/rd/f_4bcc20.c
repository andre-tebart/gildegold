/* 0x4bcc20, lays a road from the building site of object obj (name: 0x58f640) to the next road (marks its cells
   as ground type "EINFACHER_WEG" on the town map, around each cell before 0x780)
   @deep behaviour: far draft (the layout note's DDA / loop shapes are guesses), 466 code hunks - frame 0xc0 vs 0xc8, 36 slot hunks, the marking loops, the path merge and the dummy/height steps differ in shape throughout; Q2 also blocked by ai_Unkfd7f78, a flag byte inside AIMAPREF[0]+0x32c only tested as bit 1 before d3fl_MakeDivide, no other reader and no evidenced role */

int rd_BuildRoad(char *obj, int name)
{
    G15BKFIND f;
    G15BK *bk;
    float v[3];
    float ax, az, bx, bz;
    float dx, dz, ex, ez;
    float x, z, step;
    int i, j, k;
    char *o;
    int px, pz;
    char buf[0x40];

    rd_Road.map = (G15MAP *)ai_MapRefs;
    if (ai_MapRefs == 0)
        return 0;
    rd_Road.grid = d3sm_Create((struct D3TERRAIN *)ai_MapRefs, ((G15MAP *)ai_MapRefs)->w, 2);
    if (rd_Road.grid == 0)
        return 0;
    rd_Road.prunk = -1;
    rd_Road.simple = -1;
    for (i = 0; i < 8; i++) {
        strcpy(buf, rd_Road.map->ground[i]);
        _strupr(buf);
        if (strstr(buf, "EINFACHER_WEG"))
            rd_Road.simple = i;
        if (strstr(buf, "PRUNK_WEG"))
            rd_Road.prunk = i;
    }
    if (rd_Road.prunk == -1)
        rd_Road.prunk = 1;
    if (rd_Road.simple == -1)
        rd_Road.simple = 2;
    rd_AddBauplaetze(&rd_Road);
    if (!ai_BuildingName(name, f.name)) {
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
    d3_LocalToWorld(obj, v, f.pos);
    f.found = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x180, rd_FindSiteCB, &f);
    if (f.found == 0)
        goto fail;
    bk = rd_GetBKSize(f.found);
    if (bk == 0) {
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    v[0] = rd_Road.map->cw + bk->x0;
    v[1] = bk->y0;
    v[2] = rd_Road.map->ch + bk->z0;
    rd_WorldToCell(&ax, &az, v, f.found, &rd_Road);
    v[0] = bk->x1 - rd_Road.map->cw;
    v[1] = bk->y0;
    v[2] = rd_Road.map->ch + bk->z0;
    rd_WorldToCell(&bx, &bz, v, f.found, &rd_Road);
    if (!rd_PathToRoad(&rd_Road, (int)ax, (int)az, -1, -1)) {
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    memcpy(rd_Road.path2, rd_Road.path, sizeof(rd_Road.path));
    rd_Road.n2 = rd_Road.n;
    if (!rd_PathToRoad(&rd_Road, (int)bx, (int)bz, -1, -1))
        goto fail;
    if (rd_Road.n2 > rd_Road.n && rd_Road.n2 >= 5) {
        memcpy(rd_Road.path2, rd_Road.path, sizeof(rd_Road.path));
        rd_Road.n2 = rd_Road.n;
    } else if (rd_Road.n2 < 5) {
        memcpy(rd_Road.path2[rd_Road.n2], rd_Road.path, sizeof(rd_Road.path));
        rd_Road.n2 += rd_Road.n;
    }
    dx = bx - ax;
    dz = bz - az;
    ex = (float)fabs(dx);
    ez = (float)fabs(dz);
    if (ex <= ez) {
        dx = dx / ez;
        step = dz > 0.0f ? 1.0f : -1.0f;
        x = ax;
        z = az;
        while ((int)z != (int)bz) {
            rd_Road.path2[rd_Road.n2][0] = (unsigned char)(int)x;
            rd_Road.path2[rd_Road.n2][1] = (unsigned char)(int)z;
            rd_Road.n2++;
            rd_Road.grid->cells[(rd_Road.map->w * (int)z + (int)x) * 7] = 6;
            x += dx;
            z += step;
        }
    } else {
        dz = dz / ex;
        step = dx > 0.0f ? 1.0f : -1.0f;
        x = ax;
        z = az;
        while ((int)x != (int)bx) {
            rd_Road.path2[rd_Road.n2][0] = (unsigned char)(int)x;
            rd_Road.path2[rd_Road.n2][1] = (unsigned char)(int)z;
            rd_Road.n2++;
            rd_Road.grid->cells[((int)z * rd_Road.map->w + (int)x) * 7] = 6;
            x += step;
            z += dz;
        }
    }
    o = d3_FindObject(obj, 0x100, 0, "dummy_TUER");
    if (o == 0)
        o = obj;
    d3_PointToWorld(o, o + 0x4c, v);
    if (d3_GetHeightAt((char *)rd_Road.grid, v, &px, &pz, 0)
        && rd_PathToRoad(&rd_Road, px, pz, -1, -1)) {
        d3sm_Destroy(rd_Road.grid);
        if (ai_MapSize < 0x780) {
            for (i = 0; i < rd_Road.n2; i++) {
                for (j = -1; j < 2; j++) {
                    for (k = -1; k < 2; k++)
                        rd_Road.map->cells[(rd_Road.path2[i][1] + j) * rd_Road.map->w
                            + rd_Road.path2[i][0] + k] = (unsigned char)rd_Road.simple;
                }
            }
            for (i = 0; i < rd_Road.n; i++) {
                for (j = -1; j < 2; j++) {
                    for (k = -1; k < 2; k++)
                        rd_Road.map->cells[(rd_Road.path[i][1] + j) * rd_Road.map->w
                            + rd_Road.path[i][0] + k] = (unsigned char)rd_Road.simple;
                }
            }
        } else {
            for (i = 0; i < rd_Road.n2; i++)
                rd_Road.map->cells[rd_Road.path2[i][1] * rd_Road.map->w + rd_Road.path2[i][0]]
                    = (unsigned char)rd_Road.simple;
            for (i = 0; i < rd_Road.n; i++)
                rd_Road.map->cells[rd_Road.path[i][1] * rd_Road.map->w + rd_Road.path[i][0]]
                    = (unsigned char)rd_Road.simple;
        }
        if ((ai_Unkfd7f78 & 2) == 0)
            d3fl_MakeDivide(rd_Road.map);
        return 1;
    }
fail:
    d3sm_Destroy(rd_Road.grid);
    return 0;
}
