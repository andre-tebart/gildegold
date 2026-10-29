/* 0x411d10, the four lower corners of the box around the mesh keys up to time t */

int d3t_GetFootprint(D3OBJECT *obj, float t, D3VECTOR *c0, D3VECTOR *c1, D3VECTOR *c2, D3VECTOR *c3)
{
    D3VECTOR4 v;
    float mn[4];
    float mx[4];
    D3MESHKEY *k;
    int i, j;

    if (obj->type != 4)
        return 1;
    d3_TransformSurf(obj);
    k = obj->info->keys;
    for (i = 0; i < obj->info->nkeys; k++, i++) {
        if (k->time <= t) {
            mn[0] = k->pos[0];
            mn[1] = k->pos[1];
            mn[2] = k->pos[2];
            mx[0] = k->pos[0];
            mx[1] = k->pos[1];
            mx[2] = k->pos[2];
            break;
        }
    }
    for (; i < obj->info->nkeys; i++, k++) {
        if (k->time <= t) {
            for (j = 0; j < 3; j++) {
                mn[j] = min(mn[j], k->pos[j]);
                mx[j] = max(mx[j], k->pos[j]);
            }
        }
    }
    v.x = mn[0];
    v.y = mn[1];
    v.z = mn[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c0);
    v.x = mx[0];
    v.y = mn[1];
    v.z = mn[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c1);
    v.x = mx[0];
    v.y = mn[1];
    v.z = mx[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c2);
    v.x = mn[0];
    v.y = mn[1];
    v.z = mx[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c3);
    return 0;
}
