/* 0x4535b0, like d3_RelightObject for an object whose vertices and normals were just posed by the animation
   frame: lights of group mask 0xe, the frame's radius as the reach margin */

void d3_RelightObjectFrame(D3OBJECT *obj, struct D3ANIMFRAME *frame)
{
    D3LINFO *info;
    D3LCTX ctx;
    D3VECTOR4 c;
    int i, g;
    float s;
    D3LVERT *k;

    if (!(obj->stateFlags & 2)) {
        if (!obj->info) {
            obj->info = d3_SelectSurf(obj);
            if (!obj->info)
                return;
        }
        info = D3_L(obj)->info;
        d3_Busy++;
        d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
        if (frame)
            d3_LightRadiusMargin = ((D3LFRAME *)frame)->radius;
        else
            d3_LightRadiusMargin = 0.0f;
        s = d3_LightRadiusMargin + info->ops->radius;
        D3_L(obj)->radius2 = s * s;
        d3_FrameNormals(obj, (D3OBJINFO *)info, frame);
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
        d3_ForEachObject(obj->scene, 0, 0xe, (D3OBJECTCB)d3_LightCollectCB, &ctx);
        if (ctx.count > 0) {
            ctx.list = (D3OBJECT **)mem_Alloc(ctx.count << 2, "d3_light:cache");
            ctx.count = 0;
            d3_ForEachObject(obj->scene, 0, 0xe, (D3OBJECTCB)d3_LightCollectCB, &ctx);
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
                s = c.x;
                if (!(s > c.y))
                    s = c.y;
                if (!(s > c.z))
                    s = c.z;
                if (s > 255.0) {
                    float q = 255.0f / s;
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
            }
        } else {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                d3_FtoI(&g, c.x * 0.3f + c.y * 0.59f + c.z * 0.11f);
                k[i].base.r = min((unsigned int)g, 0xff);
            }
        }
        D3_L(obj)->time = d3_Time;
        D3_L(obj)->mesh->lightGeneration = d3_LightGeneration;
        d3_Busy--;
    }
}
