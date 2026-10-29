/* 0x457db0, bounds of an object from its transformed box (also stored in its material) */

char d3q_GetBounds(D3OBJECT *obj, float *min, float *max)
{
    D3BOXVERT *v;
    int i;

    if (!(obj->stateFlags & 0x80))
        d3_UpdateWorldMatrix(obj, d3_Camera, 0);
    v = d3_GetBoxVerts(obj, (struct D3OBJINFO *)D3Q_MAT(obj)->list, D3Q_MAT(obj)->box);
    if (v) {
        min[0] = 1e10f;
        min[1] = 1e10f;
        min[2] = 1e10f;
        max[0] = -1e10f;
        max[1] = -1e10f;
        max[2] = -1e10f;
        for (i = 8; i != 0; i--, v++) {
            min[0] = min(min[0], v->x);
            min[1] = min(min[1], v->y);
            min[2] = min(min[2], v->z);
            max[0] = max(max[0], v->x);
            max[1] = max(max[1], v->y);
            max[2] = max(max[2], v->z);
        }
        D3Q_MAT(obj)->min.x = min[0];
        D3Q_MAT(obj)->min.y = min[1];
        D3Q_MAT(obj)->min.z = min[2];
        D3Q_MAT(obj)->max.x = max[0];
        D3Q_MAT(obj)->max.y = max[1];
        D3Q_MAT(obj)->max.z = max[2];
        return 1;
    }
    return 0;
}
