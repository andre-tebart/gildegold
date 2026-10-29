/* 0x4c9830, handler of type 0x2f: the person "sp_<data[0]>" gets a place (building data[4] / room data[8]) and sits
   down, or gets the action 0x39 */

int ai_Unk4c9830(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p, *o, *r;
    int i = 0;
    char *b = 0;
    int save;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    if (G17_I(*m, 8) != -1)
        i = gm_FindObjectInst(G17_I(*m, 8));
    else
        b = gm_FindBuildingById(G17_I(*m, 4));
    save = d3_CurrentScene;
    if (G17_I(*m, 8) != -1) {
        i = init_ObjectSceneUniverse(i);
        if (i == -1)
            return 2;
    } else {
        if (G17_I(*m, 4) != -1) {
            i = init_BuildingSceneUniverse(b);
            if (i == -1)
                return 2;
        } else
            i = 0;
    }
    if (init_SceneDirty[i * 0x3e0] != 0)
        return 3;
    if (m->data[0xc] != 0) {
        d3if_SelectScene(i, 1);
        o = d3_FindObject(0, 0x160, 0, &m->data[0xc]);
        d3if_SelectScene(save, 1);
        if (o == 0)
            return 4;
        ch_StandUp(p, "sitzend/aufstehen");
        r = ch_WalkToObject(p, o);
        if (r != 0)
            memcpy(r + 0x70, ai_Unk60c4a0, 20);
    } else {
        ch_StandUp(p, "sitzend/aufstehen");
        ch_AddAction(p, 0x39, i);
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
