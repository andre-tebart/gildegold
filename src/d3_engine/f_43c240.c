/* 0x43c240, frees all scenes and the engine: the shadow object, then every scene's object, emitter, terrain,
   sky and event lists, then the two global lists (mesh drivers via d3_StockListHead / d3_AnimListHead), the
   sort buffers, the mirror planes, the sky, the shadows, the textures and finally Direct3D. */

void d3_Shutdown(void)
{
    int cur;
    int i;

    if (d3_EngineReady) {
        d3_EngineReady = 0;
        cur = d3_CurrentScene;
        D3_MESH(d3s_Object)->surf[0].info.nkeys = (d3s_MaxPolys >> 4) * 25;
        D3_MESH(d3s_Object)->surf[0].info.ntris = d3s_MaxPolys * 2;
        d3_DisposeObject(d3s_Object);
        d3s_Object = 0;
        for (i = 0; i < D3_MAX_SCENES; i++) {
            if (i != cur && d3_Scenes[i].first) {
                d3if_SelectScene(i, 1);
                while (d3_TopFirst != &d3_ObjEnd)
                    d3_DisposeObject(d3_TopFirst);
                d3if_MegaCam = 0;
                while (d3_ParHead != &d3_ParEnd)
                    par_Delete(d3_ParHead);
                if (d3_Terrain) {
                    d3fl_FreeMap(d3_Terrain);
                    d3_Terrain = 0;
                }
                if (d3_Sky) {
                    d3sky_Destroy(d3_Sky);
                    d3_Sky = 0;
                }
                if (d3_DefaultScene->events) {
                    mem_Free(d3_DefaultScene->events);
                    d3_DefaultScene->events = 0;
                }
            }
        }
        if (cur != d3_CurrentScene)
            d3if_SelectScene(cur, 1);
        while (d3_TopFirst != &d3_ObjEnd)
            d3_DisposeObject(d3_TopFirst);
        if (d3_Terrain) {
            d3fl_FreeMap(d3_Terrain);
            d3_Terrain = 0;
        }
        if (d3_Sky) {
            d3sky_Destroy(d3_Sky);
            d3_Sky = 0;
        }
        par_DeleteAll();
        if (d3_DefaultScene->events) {
            mem_Free(d3_DefaultScene->events);
            d3_DefaultScene->events = 0;
        }
        while (d3_StockListHead != &d3_StockListEnd)
            d3_StockListHead->Release(d3_StockListHead);
        while (d3_AnimListHead != d3_AnimListEnd)
            d3anim_Release(d3_AnimListHead, 1);
        if (d3_Mirror.obj && d3_Mirror.planes) {
            mem_Free(d3_Mirror.planes);
            d3_Mirror.planes = 0;
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        mem_Free(d3_SortBuf);
        d3_SortBuf = 0;
        mem_Free(d3_SortTmp);
        d3_SortTmp = 0;
        d3sky_Free();
        d3_ExitShadows();
        d3fl_Exit();
        tex_Exit();
        d3_ShadeReset();
        d3_AnimPath = 0;
        if (d3_D3DActive)
            d3d_Exit();
        dd_Exit();
    }
}
