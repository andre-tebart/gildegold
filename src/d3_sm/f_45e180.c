/* 0x45e180, d3_ForEachObject callback: grows box[0..1] (min, max) by the object's vertices */

char d3sm_BoundsCB(D3OBJECT *obj, D3VECTOR4 *box)
{
    D3SMMESH *m = (D3SMMESH *)obj->info;
    D3SMVERT *v;
    int i, k;

    if (m && (!(obj->stateFlags & 0xc) || (obj->stateFlags & 0x10))) {
        for (i = 0, v = m->keys; i < ((D3SMMESH *)obj->info)->nkeys; i++, v++) {
            for (k = 0; k < 3; k++) {
                (&box[0].x)[k] = min((&box[0].x)[k], (&v->x)[k]);
                (&box[1].x)[k] = max((&box[1].x)[k], (&v->x)[k]);
            }
        }
    }
    return 1;
}
