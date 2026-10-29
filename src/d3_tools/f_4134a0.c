/* 0x4134a0, on: sets every vertex color of obj to grey, off: lets 0x453280 restore them */

void d3t_SetGrey(D3OBJECT *obj, int on)
{
    D3TRI *tri;
    int i, j;

    if (obj && obj->info) {
        if (on) {
            tri = obj->info->tris;
            for (i = 0; i < obj->info->ntris; tri++, i++) {
                for (j = 0; j < 3; j++) {
                    ((D3VERTEX *)tri->v[j])->r = 0x80;
                    ((D3VERTEX *)tri->v[j])->g = 0x80;
                    ((D3VERTEX *)tri->v[j])->b = 0x80;
                }
            }
            obj->stateFlags |= 2;
            obj->state |= 4;
        } else {
            obj->stateFlags &= ~2;
            d3_RelightObject(obj);
            obj->state |= 4;
        }
    }
}
