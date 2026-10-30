/* 0x4478e0, makes scene `index` the current one: saves the engine's global state into the old scene and
   loads the new scene's; with `keep` the objects, sky and map of the old state stay as they are
   @deep Q2 d3if_Unk5fb5c0 / d3if_Unk5fb5c4: the same write-only scene pair as d3if_Reset (.text scan: only this function's two save/restore pairs and d3if_Reset's stores; .data initials 4 / 0xf; no reader, no pointer to it, no meaning derivable). The other 12 identifiers (d3s_ResetShadows/45e620, d3sm_Shadow, d3_Mirror.obj, d3_Mirror.info/08, d3if_Unk6339db/dc, d3if_Unk633dcc, d3if_Unkfba944, d3if_Unkfc29bc/40) still need reading; behaviour is already reached (the 22 code hunks are moved instructions per diffclass) */

char d3if_SelectScene(int index, char keep)  /* @original: the definition returns char and takes a char keep (byte-matched); the headers declare void (int, int) */
{
    D3OBJECT *cam;

    if (index >= 0x40)
        return 0;
    if (index != d3_CurrentScene) {
        if (!keep && d3_Mirror.obj && d3_Mirror.planes) {
            mem_Free(d3_Mirror.planes);
            d3_Mirror.planes = 0;
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        D3IF_SCENE(d3_CurrentScene).first = d3_TopFirst;
        D3IF_SCENE(d3_CurrentScene).last = d3_TopLast;
        if (D3IF_SCENE(d3_CurrentScene).first == &d3_ObjEnd && D3IF_SCENE(d3_CurrentScene).last == &d3_ObjEnd) {
            D3IF_SCENE(d3_CurrentScene).last = 0;
            D3IF_SCENE(d3_CurrentScene).first = 0;
        }
        D3IF_SCENE(d3_CurrentScene).par1 = d3_ParHead;
        D3IF_SCENE(d3_CurrentScene).par2 = d3_ParTail;
        if (D3IF_SCENE(d3_CurrentScene).par1 == &d3_ParEnd && D3IF_SCENE(d3_CurrentScene).par2 == &d3_ParEnd) {
            D3IF_SCENE(d3_CurrentScene).par2 = 0;
            D3IF_SCENE(d3_CurrentScene).par1 = 0;
        }
        D3IF_SCENE(d3_CurrentScene).map = d3_Terrain;
        D3IF_SCENE(d3_CurrentScene).sky = d3_Sky;
        D3IF_SCENE(d3_CurrentScene).sceneFlags = d3_SceneFlags;
        D3IF_SCENE(d3_CurrentScene).amb.x = d3if_AmbR;
        D3IF_SCENE(d3_CurrentScene).amb.y = d3if_AmbG;
        D3IF_SCENE(d3_CurrentScene).amb.z = d3if_AmbB;
        D3IF_SCENE(d3_CurrentScene).dir.x = d3if_Dir[0];
        D3IF_SCENE(d3_CurrentScene).dir.y = d3if_Dir[1];
        D3IF_SCENE(d3_CurrentScene).dir.z = d3if_Dir[2];
        D3_COPY(D3IF_SCENE(d3_CurrentScene).levels, offset d3if_Levels, sizeof(D3IF_SCENE(d3_CurrentScene).levels));
        D3IF_SCENE(d3_CurrentScene).fog.a = d3_FogColor;
        D3IF_SCENE(d3_CurrentScene).fog.b = d3if_Unkfc29bc;
        D3IF_SCENE(d3_CurrentScene).fog.c = d3if_Unkfc5340;
        D3_COPY(D3IF_SCENE(d3_CurrentScene).fogs, offset d3if_Fog, sizeof(D3IF_SCENE(d3_CurrentScene).fogs));
        D3IF_SCENE(d3_CurrentScene).unk3cc = d3if_Unk5fb5c0;
        D3IF_SCENE(d3_CurrentScene).unk3d0 = d3if_Unk5fb5c4;
        D3IF_SCENE(d3_CurrentScene).hardware = d3_Hardware;
        cam = (D3OBJECT *)d3if_MegaCam;
        D3IF_SCENE(d3_CurrentScene).cam = cam;
        if (cam) {
            D3IF_SCENE(d3_CurrentScene).campos.x = cam->pos.x;
            D3IF_SCENE(d3_CurrentScene).campos.y = cam->pos.y;
            D3IF_SCENE(d3_CurrentScene).campos.z = cam->pos.z;
            D3IF_SCENE(d3_CurrentScene).camrot.x = cam->rotation.x;
            D3IF_SCENE(d3_CurrentScene).camrot.y = cam->rotation.y;
            D3IF_SCENE(d3_CurrentScene).camrot.z = cam->rotation.z;
        }
        if (!D3IF_SCENE(index).first)
            d3if_InitScene(&D3IF_SCENE(index));
        d3_ParHead = D3IF_SCENE(index).par1;
        d3_Sky = D3IF_SCENE(index).sky;
        d3_TopLast = D3IF_SCENE(index).last;
        d3_SceneFlags = D3IF_SCENE(index).sceneFlags;
        d3_Terrain = D3IF_SCENE(index).map;
        d3if_Unk5fb5c4 = D3IF_SCENE(index).unk3d0;
        d3_TopFirst = D3IF_SCENE(index).first;
        d3if_AmbG = D3IF_SCENE(index).amb.y;
        d3_ParTail = D3IF_SCENE(index).par2;
        d3if_AmbR = D3IF_SCENE(index).amb.x;
        d3if_Unk5fb5c0 = D3IF_SCENE(index).unk3cc;
        d3if_AmbB = D3IF_SCENE(index).amb.z;
        d3if_Dir[0] = D3IF_SCENE(index).dir.x;
        d3_Camera = 0;
        d3if_Dir[1] = D3IF_SCENE(index).dir.y;
        d3if_Dir[2] = D3IF_SCENE(index).dir.z;
        d3if_Gray = d3if_AmbR * 0.3f + d3if_AmbG * 0.59f + d3if_AmbB * 0.11f;
        D3_COPY_TO(offset d3if_Fog, D3IF_SCENE(index).fogs, sizeof(D3IF_SCENE(index).fogs));
        D3_COPY_TO(offset d3if_Levels, D3IF_SCENE(index).levels, sizeof(D3IF_SCENE(index).levels));
        d3_CurrentScene = index;
        d3_DefaultScene = (D3SCENE *)&D3IF_SCENE(index);
        if (d3if_Unkfba944 == d3if_MegaCam)
            d3if_Unkfba944 = (int)D3IF_SCENE(index).cam;
        d3if_MegaCam = (int)D3IF_SCENE(index).cam;
        if (d3if_MegaCam) {
            d3_SetObjectPos((D3OBJECT *)d3if_MegaCam, &D3IF_SCENE(index).campos);
            d3_SetObjectRot((D3OBJECT *)d3if_MegaCam, &D3IF_SCENE(index).camrot);
            d3_SetCamera((D3OBJECT *)d3if_MegaCam);
        }
        if (!keep) {
            if (!d3_Terrain) {
                if (d3sm_ShadowV)
                    d3sm_Build(d3sm_ShadowV, 0);
                else {
                    d3sm_ShadowV = d3sm_Create(0, -d3if_Unk633dcc, 1);
                    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, d3s_ResetShadows);
                }
            } else if (d3sm_ShadowV) {
                d3sm_Destroy(d3sm_ShadowV);
                d3sm_ShadowV = 0;
            }
            d3_SortPtr = d3_SortBuf;
            d3_SortCount = 0;
            ((void (*)(unsigned long, unsigned long, unsigned long))d3_SetFog)(D3IF_SCENE(index).fog.b, D3IF_SCENE(index).fog.c, D3IF_SCENE(index).fog.a);  /* @original: the three floats as dwords, no conversion */
        }
    }
    if (!keep) {
        d3if_Unk6339db = 0;
        if (d3_Hardware && d3d_D3D && d3_Busy <= 0 && (d3if_Unk6339dc++ & 0xf) == 0 &&
        (d3d_hr = d3d_D3D->lpVtbl->EvictManagedTextures(d3d_D3D)) != 0)
            d3d_Error(__FILE__, 682, "d3d_D3D->lpVtbl->EvictManagedTextures(d3d_D3D)");
        d3if_Unk6339db = 1;
    }
    return 1;
}
