/* 0x40a9c0, action callback 0x39: walks to a random dummy object if the character is in scene args[0] */

void ch_ActWalk2RndDummy(CHACTION *a)
{
    struct D3HMAP *h;
    int cur, scene;
    D3OBJECT *obj;
    CHACTION *w;
    int x, z;
    float y;
    D3VECTOR4 pos;

    if (!a->count) {
        h = ch_GetHMap(a->ch);
        cur = d3_CurrentScene;
        scene = d3t_SceneIndex(a->ch->scene);
        if (scene != a->args[0]) {
            ch_RemoveAction(a);
            return;
        }
        d3if_SelectScene(scene, 1);
        obj = ch_FindDummy();
        if (obj && h) {
            d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
            if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y)) {
                w = ch_InsertAction(a->ch, a, 0x2d, x, z, scene);
                w->u.walk.flags |= CH_WALK_DEST;
                w->u.walk.dest.x = pos.x;
                w->u.walk.dest.y = pos.y;
                w->u.walk.dest.z = pos.z;
                strcpy(CHACT_NAME70(w), "Walk2RndDummy");
            }
        }
        ch_RemoveAction(a);
        d3if_SelectScene(cur, 1);
    }
}
