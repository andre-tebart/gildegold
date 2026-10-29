/* 0x405b30, action callback: walks to the gate object name2 in room unk170, then changes the scene */

void ch_ActUsingGate(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    CHACTION *b;
    int cur, scene;
    D3OBJECT *obj;
    struct D3HMAP *h;
    int x, z;
    float y;
    D3VECTOR4 pos;
    char buf[0x200];

    if (a->count)
        return;
    cur = d3_CurrentScene;
    scene = d3t_SceneIndex(ch->scene);
    if (scene == CH_ACTOBJ(a)->unk170) {
        ch_RemoveAction(a);
        return;
    }
    d3if_SelectScene(scene, 1);
    obj = d3_FindObject(0, 0x120, 0, CH_ACTOBJ(a)->name2);
    if (obj) {
        h = d3_Scenes[scene].hmap;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
        if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y))
            ch_InsertAction(ch, a, 0x2d, x, z, scene);
        else
            sprintf(buf, "UsingGate: d3sm_3D2SuperMap() failed... scene:%s   object:%s", ch->scene, obj);
    }
    d3if_SelectScene(cur, 1);
    if (ch->obj->type != 1)
        ch_InsertAction(ch, a, 0x38, 0);
    b = ch_InsertAction(ch, a, 0x33, CH_ACTOBJ(a)->unk170, CH_ACTOBJ(a)->unk174, CH_ACTOBJ(a)->unk178);
    strcpy(b->u.name, a->u.name);
    ch_InsertAction(ch, b, 0x38, 1);
    ch_RemoveAction(a);
}
