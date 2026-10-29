/* 0x425470, one-directional A*: expands from the start node alone until it reaches the goal;
   the start node index, or 0xffff when no path exists. Sets the path_ globals like path_Search2.
   @deep behaviour only: size 784 = original, remaining code hunks are register allocation and the same double-compare idiom (and eax,0x100 vs test ah,0x5/jnp, loop and eax,0x4100) as path_Search2; readable met */

unsigned short path_Search1(PMAP *map, int sx, int sy, short ex, int ey, int type)
{
    int k;
    unsigned int a;
    unsigned short b;

    path_Shift = 0;
    path_Mask = map->size - 1;
    for (k = map->size; k >>= 1; )
        path_Shift++;
    path_DY[0] = 0;
    path_DY[1] = -map->size;
    path_DY[2] = 0;
    path_DY[3] = map->size;
    path_DY[4] = -map->size;
    path_DY[5] = -map->size;
    path_DY[6] = map->size;
    path_DY[7] = map->size;
    path_Count = 0;
    a = (sy << path_Shift) + sx;
    path_Start = a;
    path_Goal = (short)(ey << path_Shift) + ex;
    path_Stamp++;
    PATH_INITNODE(&map->nodes[(unsigned short)a], 5);
    if (path_Start != path_Goal) {
        b = path_Goal;
        PATH_INITNODE(&map->nodes[b], 9);
        path_Tab1Sel = path_Tab1[type];
        path_Tab2Sel = path_Tab2[type];
        path_Costs = path_Terrain[type];
        if (path_Costs[map->nodes[(unsigned short)a].type] >= 0.0 && path_Costs[map->nodes[b].type] >= 0.0) {
            path_Weight = 0.0f;
            for (k = 1; k < 15; k++)
                if (path_Costs[k] >= 0.0)
                    path_Weight += path_Costs[k];
            path_Weight = 7.0f;
            path_Target = path_Goal;
            path_SetFlag = 4;
            path_TestFlag = 8;
            a = path_Expand(map, a);
            while ((short)a != -1) {
                if ((unsigned short)a == path_Goal) {
                    path_Reverse(map, a);
                    return path_Start;
                }
                if ((map->nodes[(unsigned short)a].flags & 0xc) == 0xc) {
                    path_Reverse(map, a);
                    map->nodes[(unsigned short)a].parent = path_Meet;
                    return path_Start;
                }
                path_Count++;
                path_SetFlag = 4;
                path_TestFlag = 8;
                path_Target = path_Goal;
                a = path_Expand(map, a);
            }
        }
        a = 0xffff;
    }
    return a;
}
