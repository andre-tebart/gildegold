/* 0x43e200, draws the default scene @deep Q2 only (byte already): 12 write-only engine statics (d3_Unk633470/474/478/47c/480/484/488/494/498, d3_MirrorObject, d3_Unkfb9928, d3_Unkfc2dc0) are stored from the named counters (d3_SortCount, d3_TriCount, d3_VertCount, d3_DrawnVerts, d3_TextureSwitches) and never read anywhere in src/ (grep: only d3_engine.c + d3_engine.h; d3_MirrorObject is also written 0/obj in d3_mir.c) - no reader, no data.csv alias, so no name; d3_FogByDistance's canonical alias is d3_FogByDistance (f3init.c, placeholder); d3_MirDrawObject is the unnamed d3_mir.c D3MIRROR callback (own @deep). Evidence exists for three callees but the unit's header cleanup owns them: d3fl_Draw = d3fl_Draw, d3fl_FarDist = d3fl_FarDist (d3_fl.h types D3FLMAP*), d3sky_Draw = d3sky_Draw (char (D3SKY*)); d3_engine.h has void* views of all three and d3_engine.c includes d3_sky.h only. */

void d3_Render(unsigned int mask, char flag)
{
    float d;
    D3PARTSYS *ps, *next;

    if (d3_Busy < 0)
        d3_Busy = 0;
    if (d3_EngineReady && !d3_Busy && d3_Camera) {
        d3_Busy = 1;
        if (d3_Hardware)
            d3d_Clear(&d3_Viewport);
        else
            dd_ClearB(&d3_Viewport, GetBValue(d3_FogColor), GetGValue(d3_FogColor), GetRValue(d3_FogColor));
        d3_LightKeysCulled = 0;
        d3_LightKeysVisible = 0;
        d3_MaxZ = 0.0f;
        d3_MinZ = 1e10f;
        D3_MESH(d3s_Object)->surf[0].info.ntris = 0;
        D3_MESH(d3s_Object)->surf[0].info.nkeys = 0;
        if (d3_Terrain) {
            if (d3_FogOn && d3_FogByDistance) {
                d = d3fl_FarDist(d3_Terrain, &D3_X(d3_Camera)->pos, &d3_Camera->matrix);
                if (d < d3_FogEnd2)
                    d3_SetFogRange((1.0f - (d3_FogEnd2 - d) / d3_FogEnd2) * d3_FogStart1, d);
                else
                    d3_SetFogRange(d3_FogStart1, d3_FogEnd2);
            }
            d3fl_Draw(d3_Terrain, flag);
        }
        d3_MirrorObject = 0;
        d3s_CollectLights();
        d3_Unk633498 = 0;
        d3_Unk633494 = 0;
        d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_DrawObjectCB, 0);
        if (D3_MESH(d3s_Object)->surf[0].info.ntris > 0 && d3_ShadowMode.mode)
            ((D3OBJECTCB)d3_DrawObjectCB)(d3s_Object, 0);
        for (ps = d3_ParHead; ps != &d3_ParEnd; ps = next) {
            next = ps->next;
            par_Draw(ps);
        }
        if (d3_Sky)
            d3sky_Draw(d3_Sky);
        if (flag)
            d3_LightUpdate();
        if ((d3d_ConfigBits & 0x40) == 0x40 && d3_MirrorObject && d3_Mirror.info && d3_Mirror.planes && (d3_Unkfb9928 & 1))
            d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_MirDrawObject, &d3_Mirror.obj);
        if (d3_MinZ == 1e10f)
            d3_MinZ = 0.0f;
        d3_Unk633470 = d3_SortCount;
        d3_Unk633488 = d3_DrawnVerts;
        d3_Unk633474 = d3_TriCount;
        d3_Unk633484 = d3_VertCount;
        d3_Unk633478 = d3s_Object->info->ntris;
        d3_DrawSorted();
        d3_Unk633480 = d3_Unkfc2dc0;
        d3_Unk63347c = d3_TextureSwitches;
        d3_Busy--;
    }
}
