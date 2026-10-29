/* 0x45bcd0, height of the ground below obj (a shadow of a parent, or the bottom of its box) */

float d3s_GetGround(D3OBJECT *obj, D3OBJINFO *info, D3OBJECT *light)
{
    float y;
    D3VECTOR4 pos;
    D3OBJECT *p;
    D3BOXVERT *box;
    int i;
    char found;

    y = 0.0f;
    found = 0;
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    if (obj->parent) {
        p = obj->parent;
        while (p) {
            if ((p->objectFlags & 4) && p->material) {
                for (i = 0; i < 4 && !found; i++) {
                    if (D3S_SLOTS(p->material)[i].tex
                        && D3S_SLOTS(p->material)[i].dir.x * D3S_SLOTS(p->material)[i].dir.x
                           + D3S_SLOTS(p->material)[i].dir.y * D3S_SLOTS(p->material)[i].dir.y
                           + D3S_SLOTS(p->material)[i].dir.z * D3S_SLOTS(p->material)[i].dir.z > 0.0f) {
                        y = D3S_SLOTS(p->material)[i].y;
                        found = 1;
                    }
                }
            }
            p = p->parent;
        }
    }
    if (!found && obj->material) {
        if (obj->objectFlags & 0x20)
            return pos.y - info->mesh->radius + D3S_MESH(obj->material)->groundOffset;
        box = ((D3MESHOPS *)info->mesh)->GetBox(obj, info, (char *)obj->material + 8);
        y = 1e10f;
        for (i = 8; i; i--, box++)
            y = min(y, box->y);
        y += D3S_MESH(obj->material)->groundOffset;
    }
    return y;
}
