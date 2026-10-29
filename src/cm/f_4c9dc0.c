/* 0x4c9dc0, handler of type 0x35, the "cm_ExChrGotoBuilding" texts: the person "sp_<data[0]>" is sent from its
   building (p + 0x2c) to the building data[4] (room data[0x18], id data[0x1c] / data[0x1d]): the actions 0x2d (walk
   to a position), 0x33 (gate, between 0x38 actions), 0x39 (sit down); the 0x15-byte names are copied to the action
   records. Rough draft: the control flow is from the disassembly, not checked. */

#define G17_NAME(r) strcpy((char *)(r) + 0x70, (char *)&m->data[0x2d])

int ai_Unk4c9dc0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char gate[0x80];
    float v[3];
    int A, B, W;
    char *p, *from, *to, *cb, *q, *q2, *o, *r;
    int save, scene, k, i;

    cb = 0;
    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    from = gm_BuildQuery(1, 0, 0x44);
    if (G17_I(*m, 4) != -1) {
        to = gm_BuildQuery(1, 1, G17_I(*m, 4));
        if (to == 0)
            return 1;
    } else
        to = from;
    if (*(int *)(p + 0x2c) != -1)
        cb = gm_BuildQuery(1, 1, *(int *)(p + 0x2c));
    save = d3_CurrentScene;
    if (cb != 0 && gm_IsHomeType(cb) != 0) {
        scene = d3t_SceneIndex(*(int *)(p + 0x88));
        d3if_SelectScene(scene, 1);
        if (scene < 0)
            return 0;
        if (*(int *)(p + 0x30) != -1) {
            q = gm_Query(*(int *)(cb + 0x5d), 1, 1, *(int *)(p + 0x30));
            if (q != 0 && *(unsigned short *)q != 0xfd) {
                sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)q * 0x41 + 1));
                q2 = gm_Query(*(int *)(cb + 0x5d), 1, 0, 0xfd);
                k = init_BuildingSceneUniverse(cb);
                if (k >= 0 && q2 != 0) {
                    ch_StandUp(p, "sitzend/aufstehen");
                    r = ch_UseGate(p, gate, "dummy_gate_ob_AUSSEN_RAUM", k, *(int *)(cb + 1), *(int *)(q2 + 2));
                    if (r != 0)
                        G17_NAME(r);
                }
            }
        }
        if (*(int *)(to + 1) != *(int *)(cb + 1)) {
            k = init_BuildingSceneUniverse(cb);
            if (k < 0)
                return 0;
            d3if_SelectScene(k, 1);
            o = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY");
            if (o != 0) {
                d3_PointToWorld(o, o + 0x4c, v);
                if (d3_GetHeightAt(*(int *)(ai_Unkfd7c50 + k * 0x3e0), v, &B, &A, &W) != 0) {
                    r = ch_AddAction(p, 0x2d, B, A, k);
                    if (r != 0)
                        G17_NAME(r);
                }
            }
            ch_AddAction(p, 0x38, 0);
            r = ch_AddAction(p, 0x33, 0, -1, -1);
            ch_AddAction(p, 0x38, 1);
            if (r == 0) {
                d3if_SelectScene(save, 1);
                return 1;
            }
            G17_NAME(r);
            if (*(char **)(cb + 0x61) != 0) {
                d3if_SelectScene(0, 1);
                o = d3_FindObject(*(int *)(cb + 0x61), 0x100, 0, "dummy_TUER");
                if (o == 0)
                    o = d3_FindObjectNoCase(*(int *)(cb + 0x61), 0x100, 0, "dummy_TUER");
                if (o != 0)
                    d3_PointToWorld(o, o + 0x4c, v);
                else
                    d3_PointToWorld(*(char **)(cb + 0x61), *(char **)(cb + 0x61) + 0x4c, v);
                *(float *)(r + 0x180) = v[0];
                *(float *)(r + 0x184) = v[1];
                r[0x17c] = 1;
                *(float *)(r + 0x188) = v[2];
            }
        }
    } else if (*(int *)(p + 0x2c) != *(int *)(to + 1) && *(int *)(p + 0x2c) != -1) {
        scene = d3t_SceneIndex(*(int *)(p + 0x88));
        if (scene < 0)
            return 0;
        d3if_SelectScene(scene, 1);
        ch_StandUp(p, "sitzend/aufstehen");
        o = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, v);
            if (d3_GetHeightAt(*(int *)(ai_Unkfd7c50 + scene * 0x3e0), v, &B, &A, &W) != 0) {
                r = ch_AddAction(p, 0x2d, B, A, scene);
                if (r != 0)
                    G17_NAME(r);
            }
        }
        ch_AddAction(p, 0x38, 0);
        r = ch_AddAction(p, 0x33, 0, -1, -1);
        ch_AddAction(p, 0x38, 1);
        if (r != 0) {
            G17_NAME(r);
            memcpy(r + 0xf0, "dummy_TUER", 11);
            if (cb != 0) {
                o = d3_FindObject(*(int *)(cb + 0x61), 0x100, 0, "dummy_TUER");
                if (o != 0) {
                    d3_PointToWorld(o, o + 0x4c, v);
                } else if (*(char **)(cb + 0x61) != 0) {
                    d3_PointToWorld(*(char **)(cb + 0x61), *(char **)(cb + 0x61) + 0x4c, v);
                } else
                    err_Message("cm_ExChrGotoBuilding(): start->D3Object is NULL!");
                r[0x17c] = 1;
                *(float *)(r + 0x180) = v[0];
                *(float *)(r + 0x184) = v[1];
                *(float *)(r + 0x188) = v[2];
            }
        }
    }
    if (gm_IsHomeType(to) == 0) {
        d3if_SelectScene(0, 1);
        o = 0;
        if (m->data[0xd] != 0)
            o = d3_FindObject(0, 0x100, 0, &m->data[0xd]);
        else if (*(char **)(to + 0x61) != 0) {
            if (*(int *)(p + 0x124) != 0)
                o = (char *)he_Unk555210(*(int *)(to + 0x61));
            else {
                o = d3_FindObject(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
                if (o == 0)
                    o = d3_FindObjectNoCase(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
            }
            if (o == 0)
                o = *(char **)(to + 0x61);
        }
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, v);
            if (d3_GetHeightAt(*(int *)ai_Unkfd7c50, v, &B, &A, &W) != 0) {
                r = ch_AddAction(p, 0x2d, B, A, 0);
                if (r != 0)
                    G17_NAME(r);
            }
        }
        if (m->data[0xc] == 0) {
            if (G17_I(*m, 0x18) != -1) {
                i = init_ObjectSceneUniverse(gm_Query(*(int *)(to + 0x5d), 1, 1, G17_I(*m, 0x18)));
                if (i < 0) {
                    err_Message("cm_ExChrGotoBuilding(): Could not find universe...");
                    goto sit;
                }
            } else
                i = 0;
            if (G17_I(*m, 0x18) == -1)
                err_Message("cm_ExChrGotoBuilding(): invalid room-id (cm_ID_NONE)");
            else if (gm_Query(*(int *)(to + 0x5d), 1, 1, G17_I(*m, 0x18)) == 0)
                err_Message("cm_ExChrGotoBuilding(): invalid room-id (raum==NULL)");
            ch_AddAction(p, 0x38, 0);
            r = ch_AddAction(p, 0x33, i, *(int *)(to + 1), G17_I(*m, 0x18));
            if (r != 0)
                G17_NAME(r);
            ch_AddAction(p, 0x38, 1);
sit:
            r = ch_AddAction(p, 0x39, i);
            if (r != 0)
                G17_NAME(r);
        }
    } else {
        if (*(int *)(to + 1) != *(int *)(p + 0x2c)) {
            d3if_SelectScene(0, 1);
            o = d3_FindObject(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
            if (o == 0)
                o = d3_FindObjectNoCase(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
            if (o != 0) {
                d3_PointToWorld(o, o + 0x4c, v);
                if (d3_GetHeightAt(*(int *)ai_Unkfd7c50, v, &B, &A, &W) != 0) {
                    r = ch_AddAction(p, 0x2d, B, A, 0);
                    if (r != 0)
                        G17_NAME(r);
                }
            }
        }
        if (m->data[0xc] == 0) {
            q = gm_Query(*(int *)(to + 0x5d), 1, 1, G17_I(*m, 0x18));
            if (q == 0) {
                q = gm_Query(*(int *)(to + 0x5d), 1, 0, 0xfd);
                if (q == 0) {
                    i = init_BuildingSceneUniverse(to);
                    if (i >= 0) {
                        ch_AddAction(p, 0x38, 0);
                        r = ch_AddAction(p, 0x33, i, *(int *)(to + 1), -1);
                        if (r != 0) {
                            G17_NAME(r);
                            memcpy(r + 0xf0, "dummy_TRANSPORTER", 18);
                        }
                        ch_AddAction(p, 0x38, 1);
                    }
                    goto done;
                }
            }
            if (*(unsigned short *)q == 0xfd) {
                if (*(int *)(to + 1) != *(int *)(p + 0x2c)) {
                    k = init_BuildingSceneUniverse(to);
                    if (k < 0)
                        return 0;
                    ch_AddAction(p, 0x38, 0);
                    r = ch_AddAction(p, 0x33, k, *(int *)(to + 1), *(int *)(q + 2));
                    if (r != 0) {
                        G17_NAME(r);
                        memcpy(r + 0xf0, "dummy_TRANSPORTER_ENTRY", 24);
                    }
                    ch_AddAction(p, 0x38, 1);
                    d3if_SelectScene(k, 1);
                    o = d3_FindObject(0, 0x100, 0, *(int *)(p + 0x124) != 0 ? "dummy_TRANSPORTER" : "dummy_EINGANG");
                    if (o != 0) {
                        d3_PointToWorld(o, o + 0x4c, v);
                        if (d3_GetHeightAt(*(int *)(ai_Unkfd7c50 + k * 0x3e0), v, &B, &A, &W) != 0) {
                            r = ch_AddAction(p, 0x2d, B, A, k);
                            if (r != 0)
                                G17_NAME(r);
                        }
                    }
                }
            } else {
                i = init_ObjectSceneUniverse(q);
                sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)q * 0x41 + 1));
                r = ch_UseGate(p, "dummy_EINGANG", gate, i, *(int *)(to + 1), *(int *)(q + 2));
                if (r != 0)
                    G17_NAME(r);
            }
        }
    }
done:
    d3if_SelectScene(save, 1);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 7;
        s->value = (int)p;
    }
    return 0;
}
