/* 0x412a70, sets obj on the ground (the terrain map and all other meshes under its footprint);
   top picks the highest instead of the lowest height found. Returns 0 if obj was moved. */

/* inlined into d3t_DropObject; written out there, the walk keeps o in a different register */
static __inline D3OBJECT *d3t_FirstSibling(D3OBJECT *o)
{
    while (o->prev)
        o = o->prev;
    return o;
}

int d3t_DropObject(D3OBJECT *obj, int maponly, int top)
{
    D3HQUERY q;
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *parent;

    if (parent = obj->parent) {
        d3_SetParent(obj, 0);
        d3_SetObjectRotXYZ(obj, 0, obj->rotation.y, 0);
    } else
        parent = 0;
    if (!obj->info) {
        if (parent)
            d3_SetParent(obj, parent);
        return 1;
    }
    q.obj = obj;
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    if (d3t_GetHeightRange(obj, &q.lo0, &q.hi0)) {
        d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        d3_SetObjectRot(d3_Camera, &rot);
        if (parent)
            d3_SetParent(obj, parent);
        return 1;
    }
    q.t = (q.hi0 - q.lo0) * 0.1f + q.lo0;
    if (d3t_GetFootprint(obj, q.t, (D3VECTOR *)&q.c0, (D3VECTOR *)&q.c1, (D3VECTOR *)&q.c2, (D3VECTOR *)&q.c3)) {
        d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        d3_SetObjectRot(d3_Camera, &rot);
        if (parent)
            d3_SetParent(obj, parent);
        return 1;
    }
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
    d3_SetObjectRot(d3_Camera, &rot);
    if (d3_Terrain) {
        if (d3t_GetQuadRange(d3_Terrain, &q.lo, &q.hi, (D3VECTOR *)&q.c0, (D3VECTOR *)&q.c1,
                             (D3VECTOR *)&q.c2, (D3VECTOR *)&q.c3)) {
            if (parent)
                d3_SetParent(obj, parent);
            return 1;
        }
        if (maponly) {
found:
            obj->pos.y += (top ? q.hi : q.lo) - q.lo0;
            obj->state |= 4;
            if (parent)
                d3_SetParent(obj, parent);
            return 0;
        }
    } else {
        if (maponly) {
            if (parent)
                d3_SetParent(obj, parent);
            return 1;
        }
        q.lo = 1e35f;
        q.hi = -1e35f;
    }
    q.a.x = min(min(min(q.c0.x, q.c1.x), q.c2.x), q.c3.x);
    q.a.y = q.lo0;
    q.a.z = min(min(min(q.c0.z, q.c1.z), q.c2.z), q.c3.z);
    q.b.x = max(max(max(q.c0.x, q.c1.x), q.c2.x), q.c3.x);
    q.b.y = q.hi0;
    q.b.z = max(max(max(q.c0.z, q.c1.z), q.c2.z), q.c3.z);
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    {
        D3OBJECT *o;
        int r = 1;
        o = d3t_FirstSibling(obj);
        do {
            if (o != obj)
                r &= d3t_QueryHeight(&q, o);
            o = o->next;
        } while (o);
        d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        d3_SetObjectRot(d3_Camera, &rot);
        if (!r || d3_Terrain)
            goto found;
    }
    if (parent)
        d3_SetParent(obj, parent);
    return 1;
}
