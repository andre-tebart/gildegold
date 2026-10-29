/* 0x43df60, d3_ForEachObject callback: reloads the mesh of an object. */

char d3_ReloadMeshCB(D3OBJECT *obj, D3RELOADCTX *ctx)
{
    char name[0x100];
    int len;
    char *ext;

    if (obj->material && D3_MESH(obj)->surf[0].info.mesh && obj->kind == ctx->type) {
        if (ctx->count) {
            ctx->n++;
            return 1;
        }
        if (ctx->progress)
            ctx->progress(0, ctx->i++, ctx->n);
        strcpy(name, (char *)D3_MESH(obj)->surf[0].info.mesh);
        len = strlen(name);
        if (len > 2) {
            ext = &name[len - 2];
            if (!strcmp(ext, "_0") || !strcmp(ext, "_1") || !_strcmpi(ext, "_S"))
                *ext = 0;
        }
        d3_FreeObjectData(obj);
        if (d3io_LoadObject(name, name))
            d3_AttachMesh(obj, 0, name);
        d3_UpdateObject(obj, 1);
    }
    return 1;
}
