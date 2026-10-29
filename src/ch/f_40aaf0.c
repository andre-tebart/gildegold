/* 0x40aaf0, queues a walk to the object, then a turn to the object's direction */

void ch_WalkToObjectTurn(CHARACTER *ch, D3OBJECT *obj)
{
    struct D3HMAP *h;
    CHACTION *a;
    int x, z;
    float y, ang;
    D3VECTOR4 pos, dir;

    h = ch_GetHMap(ch);
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    d3_DirToWorld(obj, &d3_ZAxis, (D3VECTOR *)&dir);
    ang = d3_VecAngle(&d3_ZAxis, (D3VECTOR *)&dir);
    if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y)) {
        a = ch_AddAction(ch, 0x2d, x, z, d3t_SceneIndex(obj->scene));
        a->u.walk.flags |= CH_WALK_DEST;
        a->u.walk.dest.x = pos.x;
        a->u.walk.dest.y = pos.y;
        a->u.walk.dest.z = pos.z;
        ch_AddAction(ch, 7, (int)(-ang * 180.0 / 3.1415927f), 1);
    }
}
