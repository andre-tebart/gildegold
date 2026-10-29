/* 0x413550, sets every vertex color of obj to r/g/b; black lets 0x453280 restore them */

void d3t_SetColor(D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b)
{
    D3TRI *tri;
    int i, j;

    if (obj && obj->info) {
        if (!r && !g && !b) {
            obj->stateFlags &= ~2;
            d3_RelightObject(obj);
            obj->state |= 4;
        } else {
            tri = obj->info->tris;
            for (i = 0; i < obj->info->ntris; tri++, i++) {
                for (j = 0; j < 3; j++) {
                    ((D3VERTEX *)tri->v[j])->r = r;
                    ((D3VERTEX *)tri->v[j])->g = g;
                    ((D3VERTEX *)tri->v[j])->b = b;
                }
            }
            obj->stateFlags |= 2;
            obj->state |= 4;
        }
    }
}
