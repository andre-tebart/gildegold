/* 0x4473f0, empties the current scene: all objects, particle systems, the map, the sky, and resets
   the lighting globals
   @deep Q2 d3if_Unk5fb5c0 / d3if_Unk5fb5c4 (D3IFSCENE.unk3cc / unk3d0): write-only state - .text scan of the whole exe finds only the one store pair in d3if_Reset and the two save/restore pairs in d3if_SelectScene, no reader, no pointer to them in .data; .data initials 4 / 0xf; no writer of D3SCENE.dirty outside these either. d3fl_FreeMap, loaded, dirty named this session */

void d3if_Reset(void)
{
    int i;
    D3IFLEVEL *l;

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
    if (((D3IFSCENE *)d3_DefaultScene)->events) {
        mem_Free(((D3IFSCENE *)d3_DefaultScene)->events);
        ((D3IFSCENE *)d3_DefaultScene)->events = 0;
    }
    d3_TopFirst = &d3_ObjEnd;
    d3_TopLast = &d3_TopHead;
    d3if_Dir[0] = 0;
    d3if_Dir[1] = 0;
    d3if_Dir[2] = 0;
    d3if_Gray = 200.0f;
    d3if_AmbR = 200.0f;
    d3if_AmbG = 200.0f;
    d3if_AmbB = 200.0f;
    for (i = 0; i < 6; i++) {
        d3if_Fog[i].a = 0;
        d3if_Fog[i].c = 0;
        d3if_Fog[i].b = 0;
    }
    for (i = 0; i < 7; i++) {
        d3if_Levels[i].amb.x = d3if_AmbR;
        d3if_Levels[i].amb.y = d3if_AmbG;
        d3if_Levels[i].amb.z = d3if_AmbB;
        d3if_Levels[i].dir.x = d3if_Dir[0];
        d3if_Levels[i].dir.y = d3if_Dir[1];
        d3if_Levels[i].dir.z = d3if_Dir[2];
        D3_COPY(d3if_Levels[i].fog, offset d3if_Fog, sizeof(d3if_Levels[i].fog));
    }
    d3if_MegaCam = 0;
    d3if_CamSeite = 0;
    d3if_CamVorne = 0;
    d3if_CamOben = 0;
    d3if_CreateCameras();
    d3_SceneFlags = 0;
    d3if_Unk5fb5c0 = 4;
    d3if_Unk5fb5c4 = 0xf;
    D3IF_SCENE(d3_CurrentScene).last = 0;
    D3IF_SCENE(d3_CurrentScene).first = 0;
    D3IF_SCENE(d3_CurrentScene).dirty = 0;
    D3IF_SCENE(d3_CurrentScene).loaded = 0;
    d3_Scenes[d3_CurrentScene].pad000[0] = 0;
}
