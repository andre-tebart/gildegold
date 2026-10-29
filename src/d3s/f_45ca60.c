/* 0x45ca60 @deep B code hunks 176, fpu 20: the original keeps all five float temps (y f x z w) in memory (frame 0x44 = lpos 24 + ldir 24 + 20) where ours keeps them in registers (frame 0x20), and esi/edi hold info and light the other way round (regs 2); the seven calls (d3_UpdateWorldMatrix, d3_PointToWorld, d3_DirToWorld, d3s_GetGround, GetBox, d3_ClipBox, d3s_Update) are the same and every non-stack constant agrees, so the box projection / clip maths is the same operations in another allocation */

void d3s_UpdateObjectAt(D3OBJECT *obj)
{
    D3OBJECT *light;
    D3OBJINFO *info;
    D3BOXVERT *box;
    D3VECTOR lpos, ldir;
    float y, f, x, z, w;
    unsigned int i;
    int k;

    obj->flags &= ~2;
    if (!(obj->stateFlags & 0x80))
        d3_UpdateWorldMatrix(obj, obj->state & 0x80 ? 0 : d3_Camera, 0);
    for (i = 0; i < d3s_NumLights; i++) {
        info = obj->info;
        light = d3s_Lights[i];
        if (!info)
            return;
        d3_PointToWorld(light, &light->pos, &lpos);
        d3_DirToWorld(light, &d3_ZAxis, &ldir);
        y = d3s_GetGround(obj, info, light);
        box = ((D3MESHOPS *)info->mesh)->GetBox(obj, info, (char *)obj->material + 8);
        for (k = 8; k; k--, box++) {
            if (!(light->objectFlags & 0x10)) {
                f = -((lpos.y - y) / (lpos.y - box->y));
                x = (lpos.x - box->x) * f + lpos.x;
                w = (lpos.y - box->y) * f + lpos.y;
                z = (lpos.z - box->z) * f + lpos.z;
            } else {
                f = -((box->y - y) / ldir.y);
                x = ldir.x * f + box->x;
                w = ldir.y * f + box->y;
                z = ldir.z * f + box->z;
            }
            x -= d3_Camera->pos.x;
            w -= d3_Camera->pos.y;
            z -= d3_Camera->pos.z;
            box->x = x * d3_Camera->matrix._11 + w * d3_Camera->matrix._21 + z * d3_Camera->matrix._31 + d3_Camera->matrix._41;
            box->y = x * d3_Camera->matrix._12 + w * d3_Camera->matrix._22 + z * d3_Camera->matrix._32 + d3_Camera->matrix._42;
            box->z = x * d3_Camera->matrix._13 + w * d3_Camera->matrix._23 + z * d3_Camera->matrix._33 + d3_Camera->matrix._43;
        }
        if (!(d3_ClipBox(box - 8, 0, 0) & 0x40)) {
            obj->flags |= 2;
            d3s_Update(obj, light, (D3TERRAIN *)d3_Terrain);
        }
    }
}
