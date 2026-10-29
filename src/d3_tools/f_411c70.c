/* 0x411c70, height range of a mesh's bounding box; returns 0 on success */

int d3t_GetHeightRange(D3OBJECT *obj, float *lo, float *hi)
{
    D3BOXVERT *v;
    int i;

    *hi = 0.0f;
    *lo = 0.0f;
    if (obj->type == 4) {
        d3_UpdateWorldMatrix(obj, d3_Camera, 0);
        v = d3_GetBoxVerts(obj, obj->info, obj->material->meshData);
        if (v) {
            *lo = v->y;
            *hi = v->y;
            for (i = 0; i < 7; i++) {
                *lo = min(*lo, v[i].y);
                *hi = max(*hi, v[i].y);
            }
            return 0;
        }
    }
    return 1;
}
