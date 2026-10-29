/* 0x448630, returns the object under screen position x, y (objects of a lower type win), 0 if none */

D3OBJECT *d3if_Pick(float x, float y, int flags, int mask)
{
    D3IFPICK p;
    D3OBJECT *obj;
    float best;
    unsigned int i;

    obj = 0;
    best = 1e10f;
    for (i = 0; i < 9; i++) {
        p.hits[i].dist = 1e10f;
        p.hits[i].obj = 0;
    }
    p.dx = d3_CenterX - x;
    p.cam = d3_Camera;
    p.mask = mask;
    p.dy = d3_CenterY - y;
    d3_ForEachObject(d3_DefaultScene, 0, flags, (D3OBJECTCB)d3if_PickCB, &p);
    for (i = 0; i < 9; i++)
        if (p.hits[i].obj && p.hits[i].dist < best) {
            obj = p.hits[i].obj;
            best = p.hits[i].dist;
        }
    return obj;
}
