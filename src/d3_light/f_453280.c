/* 0x453280, recomputes the vertex colours of obj from the lights that reach it */

char d3_RelightObject(D3OBJECT *obj)
{
    D3LINFO *info;
    D3LCTX ctx;
    D3TEXFLAGS f;
    D3VECTOR4 c;
    int i, g;
    float m;
    D3LVERT *k;

    if (!obj->info)
        obj->info = d3_SelectSurf(obj);
    info = D3_L(obj)->info;
    if (info && !(obj->stateFlags & 2)) {
        ((D3TEXPACK *)&f)->i = 0;
        f.b[0] = 0xff;
        d3_Busy++;
        d3_LightPrepare(obj);
        d3_LightRadiusMargin = 0.0f;
        k = info->keys;
        for (i = 0; i < info->nkeys; i++) {
            k[i].light.x = d3_AmbientColor.x;
            k[i].light.y = d3_AmbientColor.y;
            k[i].light.z = d3_AmbientColor.z;
        }
        ctx.count = 0;
        ctx.list = 0;
        ctx.obj = obj;
        ctx.normals = (obj->state >> 5) & 1;
        d3_ForEachObject(obj->scene, 0, 10, (D3OBJECTCB)d3_LightCollectCB, &ctx);
        if (ctx.count > 0) {
            ctx.list = (D3OBJECT **)mem_Alloc(ctx.count << 2, "d3_light:cache");
            ctx.count = 0;
            d3_ForEachObject(obj->scene, 0, 10, (D3OBJECTCB)d3_LightCollectCB, &ctx);
            d3_LightApply(obj, &ctx);
            mem_Free(ctx.list);
            ctx.list = 0;
        }
        if (d3_Hardware) {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                m = c.x;
                if (!(m > c.y))
                    m = c.y;
                if (!(m > c.z))
                    m = c.z;
                if (m > 255.0f) {
                    float q = 255.0f / m;
                    c.x *= q;
                    c.y *= q;
                    c.z *= q;
                }
                d3_FtoI(&g, c.x);
                k[i].base.r = g;
                d3_FtoI(&g, c.y);
                k[i].base.g = g;
                d3_FtoI(&g, c.z);
                k[i].base.b = g;
                k[i].color = k[i].specular;
            }
        } else {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                d3_FtoI(&g, c.x * 0.3f + c.y * 0.59f + c.z * 0.11f);
                k[i].base.r = min((unsigned int)g, 0xff);
                k[i].color = k[i].specular;
            }
        }
        d3_SetVertexColors(obj, (D3SURF *)info, 0, f);
        d3_ForEachObject(obj->scene, 0, 4, (D3OBJECTCB)d3_LightAttach, obj);
        d3_LightRadiusMargin = 0.0f;
        obj->state |= 4;
        D3_L(obj)->time = d3_Time;
        D3_L(obj)->mesh->lightGeneration = d3_LightGeneration;
        d3_Busy--;
        return 1;
    }
    return 0;
}
