/* 0x447340, reloads the mesh data of an object from the file `name` */

char d3if_Reload(D3OBJECT *obj, char *name)
{
    D3OBJECT *next, *g;

    if (obj) {
        if (obj->child) {
            d3_DisposeObject(obj->child);
            obj->child = 0;
        }
        next = obj->next;
        if (next && !(next->state & 1)) {
            g = d3_NextGroup(obj);
            d3_DisposeObject(next);
            obj->next = g;
        }
        d3_FreeMesh(obj);
        d3io_LoadObject(name, name);
        d3_AttachMesh(obj, 0, name);
        tex_ReloadAll(0, 0);
        if (D3_MESH(obj) && D3_MESH(obj)->surf[0].info.mesh) {
            d3_RelightObject(obj);
            return 1;
        }
    }
    return 0;
}
