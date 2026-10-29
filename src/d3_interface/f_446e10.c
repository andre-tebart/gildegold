/* 0x446e10, creates a mesh object named `name` (loading its mesh data), optionally as a child of `a`,
   at pos / rot; returns the object or 0 when the mesh could not be loaded */
D3OBJECT *d3_CreateObject(int a, D3VECTOR *pos, D3VECTOR *rot, char *name)
{
    D3OBJECT *obj;
    unsigned int i, n;
    D3TEXHANDLE **tex;

    obj = d3_SpawnObject(4, name);
    if (obj) {
        if (a)
            d3_AddChild((D3OBJECT *)a, obj);
        if (!d3io_FindStock(name))
            d3io_LoadObject(name, name);
        d3_AttachMesh(obj, 0, name);
        if (D3_MESH(obj) && D3_MESH(obj)->surf[0].info.mesh) {
            for (i = 0; i < D3_MESH(obj)->nsurf; i++) {
                if (D3_MESH(obj)->surf[i].info.mesh) {
                    n = D3_OPS(&D3_MESH(obj)->surf[i].info)->count;
                    tex = (D3TEXHANDLE **)D3_MESH(obj)->surf[i].texHandles;
                    for (; n > 0; n--, tex++)
                        if (*tex)
                            tex_Reload(*tex, 0);
                }
            }
            d3_SetObjectPos(obj, pos);
            d3_SetObjectRot(obj, rot);
            if (!a)
                d3_LinkTop(obj);
            return obj;
        }
        d3_DisposeObject(obj);
    }
    return 0;
}
