/* 0x50d490, frees the loaded scenes of the objects and building classes and the height maps of the 64 scenes */


void init_FreeScenes(void)
{
    int i;
    int *p;

    d3if_SelectScene(0, 0);
    if (d3_QuadTree != 0) {
        d3q_FreeNode((D3QNODE *)d3_QuadTree);
        d3_QuadTree = 0;
    }
    d3if_Reset();
    i = 0;
    do {
        if (init_ObjectUniverse[i] > 0) {
            d3if_SelectScene(init_ObjectUniverse[i], 0);
            d3if_Reset();
            init_ObjectUniverse[i] = -1;
        }
        i++;
    } while (i < 0x2dc);
    i = 0;
    do {
        if (init_BuildingUniverse[i] > 0) {
            d3if_SelectScene(init_BuildingUniverse[i], 0);
            d3if_Reset();
            init_BuildingUniverse[i] = -1;
        }
        i++;
    } while (i < 0x22);
    p = (int *)init_SceneHeightMaps;
    do {
        if (*p != 0) {
            d3sm_Destroy(*p);
            *p = 0;
        }
        p += 0xf8;
    } while ((int)p < (int)INIT_SCENE_HEIGHTMAP_END);
}
