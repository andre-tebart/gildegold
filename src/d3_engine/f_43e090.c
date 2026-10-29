/* 0x43e090, reloads the meshes of all objects of the given type: counts them first (for the progress
   callback), then reloads. */

void d3_ReloadMeshes(unsigned int mask, int type, void (*progress)(int a, int i, int n))
{
    D3RELOADCTX ctx;

    ctx.count = 1;
    ctx.i = 0;
    ctx.n = 0;
    ctx.progress = progress;
    ctx.type = type;
    d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_ReloadMeshCB, &ctx);
    ctx.count = 0;
    d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_ReloadMeshCB, &ctx);
    tex_ReloadAll(0, (int)progress);
    d3_LightInvalidate(1);
}
