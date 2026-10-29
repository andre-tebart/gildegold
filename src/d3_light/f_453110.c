/* 0x453110, d3_ForEachObject callback over the lights: pass 1 (list == 0) counts the ones that reach
   ctx->obj, pass 2 collects them */

char d3_LightCollectCB(D3OBJECT *obj, D3LCTX *ctx)
{
    if (!ctx->list) {
        D3_L(obj)->light->on = 0;
        d3_PointToRef(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
        if (obj->type == 7) {
            if (D3_L(obj)->power != 0.0 && !(obj->objectFlags & 0x10)) {
                d3_DirToRef(obj, d3_Camera, &d3_ZAxis, (D3VECTOR4 *)&D3_L(obj)->light->dir);
                ctx->count++;
                D3_L(obj)->light->on = 1;
            }
        } else {
            float r = D3_L(obj)->radius2 = D3_L(obj)->range;
            float dx = D3_L(obj)->cam.x - D3_L(ctx->obj)->cam.x;
            float dy = D3_L(obj)->cam.y - D3_L(ctx->obj)->cam.y;
            float dz = D3_L(obj)->cam.z - D3_L(ctx->obj)->cam.z;
            float s;
            if (D3_L(obj)->power != 0.0 && (s = (float)sqrt(dx * dx + dy * dy + dz * dz), s < r + D3_L(ctx->obj)->radius2)) {
                ctx->count++;
                D3_L(obj)->light->on = 1;
            }
        }
    } else if (D3_L(obj)->light->on)
        ctx->list[ctx->count++] = obj;
    return 1;
}
