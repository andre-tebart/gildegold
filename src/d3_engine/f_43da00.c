/* 0x43da00, d3_ForEachObject2 callback: the inverse of d3_InflateObject, frees the geometry (keys and
   triangles) it built for every surface; a visible surface is faded out through d3_SetObjectAlpha first. */

void d3_DeflateObject(D3OBJECT *obj)
{
    char any;
    D3TEXFLAGS f;
    int i;

    if (obj && obj->material) {
        any = 0;
        for (i = 0; i <= 3; i++) {
            if (D3_MESH(obj)->surf[i].info.nkeys > 0) {
                any = 1;
                if (D3_MESH(obj)->surf[i].flags.f.b[0] < 0xff || (D3_MESH(obj)->surf[i].flags.f.b[2] & 1)) {
                    memset(&f, 0, sizeof(f));
                    f.b[2] &= 0xfe;
                    f.b[0] = 0xff;
                    d3_SetObjectAlpha(obj, &D3_MESH(obj)->surf[i].info, f);
                }
                D3_MESH(obj)->surf[i].info.ntris = 0;
                D3_MESH(obj)->surf[i].info.nkeys = 0;
                if (D3_MESH(obj)->surf[i].info.keys) {
                    mem_Free(D3_MESH(obj)->surf[i].info.keys);
                    D3_MESH(obj)->surf[i].info.keys = 0;
                }
                if (D3_MESH(obj)->surf[i].info.tris) {
                    mem_Free(D3_MESH(obj)->surf[i].info.tris);
                    D3_MESH(obj)->surf[i].info.tris = 0;
                }
            }
        }
        obj->info = 0;
        if (any)
            d3_ForEachObject(d3_DefaultScene, 0, 0x86, (D3OBJECTCB)d3_LightRemoveRecords, obj);
    }
}
