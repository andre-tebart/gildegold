/* 0x452d90, d3_ForEachObject callback over the lights: obj gets this light */

char d3_LightAttach(D3OBJECT *light, D3OBJECT *obj)
{
    D3LCTX2 ctx;

    ctx.light = light;
    ctx.normals = (obj->state >> 5) & 1;
    d3_LightRemoveRecords(light, obj);
    if (D3_L(light)->power != 0.0) {
        d3_PointToRef(light, d3_Camera, &light->pos, (D3VECTOR4 *)&D3_L(light)->cam);
        D3_L(light)->radius2 = D3_L(light)->range * D3_L(light)->range;
        d3_LightAddObject(obj, &ctx);
    }
    return 1;
}
