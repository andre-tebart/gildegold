/* 0x4cbca0, handler of type 0x4c, "cm_ExCreateGeb": a building of kind data[8] (owner = the character data[0]) at the
   position data[9..0x14]; its model "<geb dir>/<name>[_<level>].ogr" is loaded. Rough draft. */

int ai_Unk4cbca0(G17MSG *m, G17STAT *s)
{
    char nm[0x100];
    char path[0x200];
    char path2[0x100];
    char msg[0x100], msg2[0x100], msg3[0x100];
    char tmp[0x100];
    char z[0x54];
    char zz[0x84];
    char *c, *b, *d, *o, *bp;
    int sc, save, i, n;
    int w1[3], w2[3];
    int p1, p2, p3;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    b = gm_CreateBuilding(m->data[8], *(unsigned short *)c);
    if (b == 0)
        return 1;
    ai_Unk60bd8c = *(int *)(b + 1);
    sc = ai_Unk5f8b10;
    d = ai_BTypes + (unsigned char)b[0] * 0x24d;
    strcpy(nm, d + 1);
    _strupr(nm + 2);
    for (i = 0; i < 4; i++) {
        if (i != 0)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, nm, (char)(i + 0x40));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, nm);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, path2) != 0)
            break;
    }
    if (i >= 4)
        goto done;
    save = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    p1 = G17_I(*m, 9);
    p2 = G17_I(*m, 0xd);
    p3 = G17_I(*m, 0x11);
    o = d3_LoadGroup(path, 0, 0);
    if (o == 0) {
        sprintf(msg, "cm_ExCreateGeb(): Could not load 3D-Object-Group '%s'...", path);
    } else {
        memset(z, 0, sizeof(z));
        d3_SetObjectPos(o, w1);
        d3_SetObjectRot(o, &m->data[0x19]);
        ai_Unk58feb0(o, &bp);
        init_HookObject(o, bp, 2);
        ai_BuildingName((unsigned char)bp[0], tmp);
        strcpy(z, tmp);
        sprintf(msg2, "cm_ExCreateGeb(): bauplatze name:%s", tmp);
        if (lc_GetGebaeudeBauplatz(z + 0xc, bp) != 0) {
            memset(zz, 0, sizeof(zz));
            d3_ForEachObject(ai_Unk5fb5b0, sc, 0x2c0, ai_VgFilter, zz);
            n = *(int *)(z + 0x10);
            for (i = 0; i < n; i++) {
                if (d3_QuadTree != 0)
                    d3q_RemoveObject((D3OBJECT *)*(int *)(zz + i * 4), (D3QNODE *)d3_QuadTree);
                sprintf(msg3, "cm_ExCreateGeb(): killing objectptr: %s", *(char **)(zz + i * 4));
                sprintf(msg3, "cm_ExCreateGeb(): killing objectname: %s", *(char **)(zz + 0xc + i * 4));
                d3if_Delete((struct D3OBJECT *)*(int *)(zz + 0x18 + i * 4));
            }
        } else
            err_Message("cm_ExCreateGeb(): could not lc_GetGebaeudeBauplatz()...");
        sv_Unk5282f0(b);
        rd_BuildRoad(*(int *)(b + 0x61), (unsigned char)b[0]);
    }
    d3if_SelectScene(save, 1);
done:
    if (s != 0) {
        s->state = 1;
        s->unk01 = 2;
        s->value = (int)b;
    }
    return 0;
}
