/* 0x4cac40, handler of type 0x3a, "cm_ExGebUpgrade": building data[0] goes one level up (byte 0 of the object):
   the next model "<geb dir>/<name>[_<level>].ogr" is loaded and its object group replaces the old one.
   Rough draft: the float parts and the effect call are from the disassembly, not checked. */

int ai_Unk4cac40(G17MSG *m, G17STAT *s)
{
    char nm[0x80];
    char path[0x100];
    char msg[0x180];
    char buf2[0x100];
    float v[3], u[3], w1, w2, w3, y;
    char *b, *d, *o, *sp;
    int i, save, ok;
    int e, ebx;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    d = ai_BTypes + (unsigned char)b[0] * 0x24d;
    if ((unsigned char)d[0x247] >= (unsigned char)d[0x248]) {
        err_Message("cm_ExSetGebUpgrade(): already highest level");
        return 1;
    }
    b[0]++;
    b[0x5c] = b[0x5c] + (100 - b[0x5c]) / 2;
    gm_SetCapacities(b);
    gm_StockBuildingObjects(b);
    if (d[0] == 0x1d)
        unk_Unk4eff50(b);
    strcpy(nm, ai_BTypes + (unsigned char)b[0] * 0x24d + 1);
    _strupr(nm + 2);
    for (i = 0; i < 4; i++) {
        if (d[0] == 0x21) {
            if (ai_Unk770b89[*(unsigned short *)(b + 0x27) * 0x21c] == 1)
                sprintf(path, "%sgebaeude/*%s_A.ogr", ai_DataPath, nm);
            else
                sprintf(path, "%sgebaeude/*%s_B.ogr", ai_DataPath, nm);
        } else if (i != 0)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, nm, (char)(i + 0x40));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, nm);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, buf2) != 0)
            break;
    }
    if (i < 4) {
        save = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        d3_PointToWorld(*(int *)(b + 0x61), *(char **)(b + 0x61) + 0x4c, v);
        o = (char *)d3_LoadGroup(path, 0, 0);
        if (o == 0) {
            sprintf(msg, "cm_ExGebUpgrade(): Could not load 3D-Object-Group '%s'...", path);
            err_Message(msg);
        } else {
            d3_DirToWorld(*(int *)(b + 0x61), ai_AxisPair, u);
            u[0] = 0;
            u[1] = 0;
            u[2] = 0;
            u[1] = d3_VecAngle(ai_AxisPair, u);
            d3_SetObjectPos(o, &w1);
            d3_SetObjectRot(o, &w2);
            ai_Unk58feb0(o, &w3);
            if (d3_QuadTree != 0)
                d3q_RemoveObject((D3OBJECT *)*(int *)(b + 0x61), (D3QNODE *)d3_QuadTree);
            d3if_Delete((struct D3OBJECT *)*(int *)(b + 0x61));
            if (*(int *)(o + 0x1cc) == 0)
                d3_InflateObject(o);
            if (*(int *)(o + 0x1ec) != 0) {
                if (d3t_GetBoundingSphere(o, ai_Unkfc2dec, &y, &e) != 0) {
                    ebx = (int)(y * ai_Unk5edc28);
                    sp = (char *)evt_Load("Effekte/geb_upgrade.esc");
                    if (sp != 0) {
                        evt_RunScriptArgs((EVTSCRIPT *)sp, 6, (int)w3, (int)w2, (int)y, ebx, o);
                        *(int *)(sp + 0x104) = 0;
                    }
                }
            }
            init_HookObject(o, b, 2);
            sv_Unk5282f0(b);
            if (*(int *)(b + 0x61) != 0) {
                ok = gm_ClockSeason(ai_Time);
                if (ok == 1)
                    init_TextureSet = 1;
                else if (ok == 0)
                    init_TextureSet = 0;
                else if (ok == 2)
                    init_TextureSet = 2;
                else if (ok == 3)
                    init_TextureSet = 3;
                init_TextureSetHook(*(int *)(b + 0x61));
                d3_ForEachObject2(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x2c0, unk_Unk511600);
                y = (float)ai_Unk6f236c + ai_Unk623e94;
                d3_ForEachObject(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x21e, d3_SetLightKeyCB, (void *)(int)y);
                d3_RelightObjects(*(D3OBJECT **)(b + 0x61));
                if (*(int *)(b + 0x95) != -1) {
                    sp = (char *)evt_FindById(*(int *)(b + 0x95));
                    if (sp != 0)
                        evt_Stop(sp);
                    *(int *)(b + 0x95) = -1;
                    sv_Unk528490(b);
                }
            }
        }
        d3if_SelectScene(save, 1);
    }
    gm_RefreshGoodsLists(ai_OwnPlayer);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
