/* 0x447e60, empties scene `index`: the current one with d3if_Reset, any other by switching to it (keeping the
   engine state) and freeing its objects, particle systems, map and sky */

char d3if_ClearScene(int index)
{
    int cur;

    char ret = 0;
    if (index >= 0 && index < 0x40) {
    if (index != d3_CurrentScene) {
    if (D3IF_SCENE(index).first) {
        cur = d3_CurrentScene;
        d3if_SelectScene(index, 1);
        while (d3_TopFirst != &d3_ObjEnd)
            d3_DisposeObject(d3_TopFirst);
        while ((D3PARTSYS *)d3_ParHead != &d3_ParEnd)
            par_Delete((D3PARTSYS *)d3_ParHead);
        if (d3_Terrain) {
            d3fl_FreeMap(d3_Terrain);
            d3_Terrain = 0;
        }
        if (d3_Sky) {
            d3sky_Destroy(d3_Sky);
            d3_Sky = 0;
        }
        if (D3IF_SCENE(index).events) {
            mem_Free(D3IF_SCENE(index).events);
            D3IF_SCENE(index).events = 0;
        }
        d3_SceneFlags = 0;
        d3if_MegaCam = 0;
        if (cur != d3_CurrentScene)
            d3if_SelectScene(cur, 1);
    }
    } else
        d3if_Reset();
    ret = 1;
    }
    return ret;
}
