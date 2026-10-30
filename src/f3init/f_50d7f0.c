/* 0x50d7f0, loads (or reloads) the scene of the object prototype n ("scenes/*ob_%s.ed3") into a free scene slot
   (d3t_FindFreeScene), registers the callbacks 0x50d100 / 0x50d730 on it; for an already loaded slot that is
   flagged dirty (D3SCENE.dirty) it rebuilds the scene and re-places its characters: at their stored position when
   that is the origin, else at the dummy of their transport (or the object's own dummy, "dummy_EINGANG" /
   "dummy_TRANSPORTER_ENTRY" as the fallbacks). While ai_GameState is on and the frame wait is over, a "BLACK"
   fade box covers the load.
   @deep behaviour draft, 181 code hunks, every hunk read; the rest is allocation / scheduling. (1) changed: the original stores a DWORD at [esp+0x10], ours a BYTE at [esp+0x13]; int changed tried twice (214->218 hunks before the polarity fix, 181->198 after; unitdiff 138->136) - the frame shift costs more code hunks than it gains, a frame/slot job. (2) uni*0x3e0: we keep the product in ebx across the d3_LoadScene calls, the original keeps uni in ebx and recomputes shl/sub in the loop; a separate dirty-flag symbol view (0xfd7f75) tried, no change. (3) the outer reload branch's then-block is out of line in the original (the inner if/else polarity swap was kept: 201->181). (4) register roles in the fade block (FrameMask esi vs edi). spell.py: reload 50d824 fixed by !(ai_FrameWait > 0) (214->201); code 50d824/50d8ba = the changed width; dup-tails 50d97c/50db62 = (2)/(1). Readability done: name from the init_LoadObjektScene error string, canonical callees/prototypes, SCENEOBJ.scene/transport, INIT_D3SCENE view, d3_Origin for 0x5ebba4, ch_List walk. */


void init_LoadObjectScene(int n)
{
    int k;
    char *p;
    int uni, save;
    char buf[0x100];
    union {
        D3VECTOR vec;
        char raw[0x10];
    } tmp;
    struct SCENEOBJ **q;
    struct SCENEOBJ *o;
    struct D3OBJECT *d;
    void (*cb)(int, int, int);
    unsigned fl;
    char *bx;
    char changed;

    save = d3_CurrentScene;
    p = ai_Goods + n * 0x41;
    cb = 0;
    changed = 0;
    if (ai_GameStateB != 0 && !(ai_FrameWait > 0)) {
        fl = sv_FrameMaskU & 0xfffffff7;
        bx = (char *)d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0x32, 1);
        k = (unsigned char)*bx;
        while ((k & 4) == 0) {
            sv_RunFrame(0, fl | 0x100080);
            k = (unsigned char)*bx;
        }
        d2_ReleaseFade((struct D2FADE *)bx);
        d2_ClearScreen();
        cb = sv_SceneLoadProgress;
    }
    if ((ai_GameFlags & 8) != 0 && tb_IsFunctionOff(sv_UpdateClock) == 0) {
        tb_SetFunctionOff(sv_UpdateClock, 1);
        changed = 1;
    }
    if (init_ObjectUniverse[n] == -1) {
        uni = d3t_FindFreeScene();
        if (uni == -1)
            err_Message("init_LoadObjektScene:Could not find a free universe...");
        d3if_SelectScene(uni, 0);
        sprintf(buf, "scenes/*ob_%s.ed3", p + 1);
        _strupr(buf);
        if (d3_LoadScene(buf, 0, cb) != 0) {
            d3if_SelectScene(d3_CurrentScene, 0);
            d3_ForEachObject2(d3_DefaultScene, 0, 0x1e0, init_SceneNameToPrototype);
            d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
            init_ObjectUniverse[n] = uni;
            d3if_SelectScene(save, 0);
        }
    } else {
        uni = init_ObjectUniverse[n];
        if (d3_Scenes[uni].dirty != 0) {
            d3if_SelectScene(uni, 0);
            sprintf(buf, "scenes/*ob_%s.ed3", p + 1);
            _strupr(buf);
            if (d3_LoadScene(buf, 0, cb) != 0) {
                d3if_SelectScene(d3_CurrentScene, 0);
                d3sm_Destroy(d3_DefaultScene->hmap);
                d3_DefaultScene->hmap = 0;
                ch_GetSceneHMap(d3_DefaultScene);
                d3sm_Build(d3_DefaultScene->hmap, d3_Terrain);
                d3_ForEachObject2(d3_DefaultScene, 0, 0x1e0, init_SceneNameToPrototype);
                d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
                /* the original compares the walk pointer as an int (jl): a plain pointer compare would be jb */
                for (q = ch_List; (int)q < (int)(ch_List + INIT_CHARACTER_COUNT); q++) {
                    o = *q;
                    if (o != 0 && o->scene == &d3_Scenes[uni]) {
                        /* 0x38: the character's stored position, a D3VECTOR the sources do not name yet */
                        if (d3_VecEqual((D3VECTOR *)((char *)o + 0x38), &d3_Origin, 0.5f) != 0) {
                            if ((o->transport != 0 && (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER")) != 0) || (d = ch_FindDummy()) != 0 ||
                                ((d = d3_FindObject(0, 0x100, 0, "dummy_EINGANG")) != 0 || (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY")) != 0)) {
                                /* d + 0x4c: D3OBJECT.pos, the dummy's position */
                                d3_PointToWorld(d, (D3VECTOR *)((char *)d + 0x4c), &tmp.vec);
                                ch_SetPos((struct CHARACTER *)o, &tmp.vec, 1);
                            }
                        } else {
                            ch_SetPos((struct CHARACTER *)o, (D3VECTOR *)((char *)o + 0x38), 1);
                            ch_PlaceOnGround((struct CHARACTER *)o, 1);
                        }
                    }
                }
                d3_Scenes[init_ObjectUniverse[n]].dirty = 0;
                d3if_SelectScene(save, 0);
            }
        }
    }
    if ((ai_GameFlags & 8) != 0 && changed)
        tb_SetFunctionOff(sv_UpdateClock, 0);
}
