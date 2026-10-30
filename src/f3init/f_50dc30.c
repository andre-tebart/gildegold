/* 0x50dc30, loads (or reloads) the scene of the building kind k ("scenes/*gb_%s.ed3", the name
   init_BuildingKindName) into a free scene slot (kept per building class in init_BuildingUniverse); the callbacks
   0x50d100 / 0x50d730 are registered on it. Like init_LoadObjectScene for the object scenes, plus the detail level
   update of the current terrain (init_SetDetailLevel).
   @deep far draft after the base rewrites (195 code hunks), every hunk read; readability done (draft -> readable). Gains kept: ai_FrameWait <= 0 in the new-slot branch (the original's test eax,eax / jle; 220 -> 215 code hunks) and the polarity swap of the position / dummy block (the original's then-block is out of line; 215 -> 195). Tried and reverted: int changed (the dword store at instructions 10 / 21 is the original's, but the frame shift costs more code hunks - third attempt). spell.py menu left: 50dc9d slots [+0x14]/[+0x18] (local frame slots), 50dcd4 / 50dd9e order (register permutation and call-block order), 50ddc3 width (no unsigned short local in the reading; the diff there is the d3if_SelectScene / init_SetDetailLevel call order), 50e070 dup-tail (the walk's loop tail). The rest is allocation / scheduling. */


void init_LoadBuildingScene(int k)
{
    int save, uni, n, o, cls;
    struct D3OBJECT *d;
    char changed;
    char *bx;
    char *slot;
    void (*cb)(int, int, int);
    unsigned fl;
    char buf[0x104], name[0x200], name2[0x80];
    union {
        D3VECTOR vec;
        char raw[0x10];
    } tmp;
    struct SCENEOBJ **q;
    struct SCENEOBJ *co;

    save = d3_CurrentScene;
    cb = 0;
    changed = 0;
    if ((ai_GameFlags & 8) != 0 && tb_IsFunctionOff(sv_UpdateClock) == 0) {
        tb_SetFunctionOff(sv_UpdateClock, 1);
        changed = 1;
    }
    slot = (char *)init_BuildingUniverse + (unsigned char)ai_BTypes[k * 0x24d];
    if (*slot == -1) {
        if (ai_GameState != 0 && (ai_FrameWait <= 0 || sv_PanelFlag != 0)) {
            fl = sv_FrameMask & 0xfffffff7;
            bx = (char *)d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0x32, 1);
            n = (unsigned char)*bx;
            while ((n & 4) == 0) {
                sv_RunFrame(0, fl | 0x80);
                n = (unsigned char)*bx;
            }
            d2_ClearScreen();
            d2_ReleaseFade((struct D2FADE *)bx);
            cb = sv_SceneLoadProgress;
        }
        uni = d3t_FindFreeScene();
        if (uni == -1)
            err_Message("init_LoadGebaeudeScene:Could not find a free universe...");
        d3if_SelectScene(uni, 0);
        init_BuildingKindName(k, name);
        sprintf(buf, "scenes/*gb_%s.ed3", name);
        _strupr(buf);
        sprintf(name, "init_LoadGebaeudeScene: %s", buf);
        if (d3_LoadScene(buf, 0, cb) != 0) {
            d3if_SelectScene(uni, 0);
            if (d3_Terrain != 0)
                init_SetDetailLevel((int *)d3_Terrain, 1);
            d3if_SelectScene(d3_CurrentScene, 0);
            d3_ForEachObject2(d3_DefaultScene, 0, 0x160, init_SceneNameToPrototype);
            d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
            *slot = uni;
            d3if_SelectScene(save, 0);
            goto done;
        }
    } else {
        if (d3_Scenes[*slot].dirty == 0)
            goto done;
        if (ai_GameState != 0 && !(ai_FrameWait > 0)) {
            fl = sv_FrameMask & 0xfffffff7;
            bx = (char *)d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0x32, 1);
            n = (unsigned char)*bx;
            while ((n & 4) == 0) {
                sv_RunFrame(0, fl | 0x80);
                n = (unsigned char)*bx;
            }
            d2_ClearScreen();
            d2_ReleaseFade((struct D2FADE *)bx);
            cb = sv_SceneLoadProgress;
        }
        cls = *slot;
        d3if_SelectScene(cls, 0);
        init_BuildingKindName(k, name2);
        sprintf(buf, "scenes/*gb_%s.ed3", name2);
        _strupr(buf);
        if (d3_LoadScene(buf, 0, cb) != 0) {
            if (d3_Terrain != 0)
                init_SetDetailLevel((int *)d3_Terrain, 1);
            d3if_SelectScene(d3_CurrentScene, 0);
            d3sm_Destroy(d3_DefaultScene->hmap);
            d3_DefaultScene->hmap = 0;
            ch_GetSceneHMap(d3_DefaultScene);
            d3sm_Build(d3_DefaultScene->hmap, d3_Terrain);
            d3_ForEachObject2(d3_DefaultScene, 0, 0x160, init_SceneNameToPrototype);
            d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
            for (q = (struct SCENEOBJ **)ch_List; (int)q < (int)(ch_List + INIT_CHARACTER_COUNT); q++) {
                co = *q;
                if (co != 0 && co->scene == &d3_Scenes[cls]) {
                    if (d3_VecEqual((D3VECTOR *)((char *)co + 0x38), &d3_Origin, 0.5f) != 0) {
                        if ((co->transport != 0 && (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER")) != 0) || (d = ch_FindDummy()) != 0 ||
                            ((d = d3_FindObject(0, 0x100, 0, "dummy_EINGANG")) != 0 || (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY")) != 0)) {
                            /* d + 0x4c: D3OBJECT.pos, the dummy's position */
                            d3_PointToWorld(d, (D3VECTOR *)((char *)d + 0x4c), &tmp.vec);
                            ch_SetPos((struct CHARACTER *)co, &tmp.vec, 1);
                        }
                    } else {
                        ch_SetPos((struct CHARACTER *)co, (D3VECTOR *)((char *)co + 0x38), 1);
                    }
                }
            }
            d3_Scenes[*slot].dirty = 0;
        }
    }
    d3if_SelectScene(save, 0);
done:
    if ((ai_GameFlags & 8) != 0 && changed)
        tb_SetFunctionOff(sv_UpdateClock, 0);
}
