/* 0x40a900, queues a walk to the object's position */

/* ---- 0x40a900..0x40b8a0 (worker ch6) ---- */
/* The TU ends at 0x40b580; the script builtins after it are in evtc.c. */

CHACTION *ch_WalkToObject(CHARACTER *ch, D3OBJECT *obj)
{
    CHACTION *a = 0;
    struct D3HMAP *h;
    int x, z;
    float y;
    D3VECTOR4 pos;

    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    h = obj->scene->hmap;
    if (!h)
        h = ch_GetSceneHMap(obj->scene);
    if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y)) {
        a = ch_AddAction(ch, 0x2d, x, z, d3t_SceneIndex(obj->scene));
        if (a) {
            a->u.walk.flags |= CH_WALK_DEST;
            a->u.walk.dest.x = pos.x;
            a->u.walk.dest.y = pos.y;
            a->u.walk.dest.z = pos.z;
        }
    }
    return a;
}
