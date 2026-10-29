/* 0x44cbb0, attaches the stock object `name` to obj: into surf, or (surf 0) its LODs into the mesh's
   surfaces 0..2 and the "_s" variant into surface 3 */

void d3_AttachMesh(D3OBJECT *obj, int unk, char *name)
{
    D3SURF *surf = (D3SURF *)unk;
    char buf[0x100];
    int i, n;

    if (surf) {
        if (d3io_FindStock(name)) {
            if (!obj->material)
                d3_AddObjectDrawData(obj);
            d3io_AttachStock(obj, surf, name);
        }
    } else {
        d3io_LodName(name, 0, 0, buf, 0);
        if (d3io_FindStock(buf)) {
            if (!obj->material)
                d3_AddObjectDrawData(obj);
            d3io_AttachStock(obj, &D3_MESH(obj)->surf[0], buf);
            if (d3io_LodName(name, 0, -1, buf, 0) && d3io_FindStock(buf))
                d3io_AttachStock(obj, &D3_MESH(obj)->surf[3], buf);
            if (d3io_Lod.mode == 1) {
                for (i = 1, n = 1; i < 3; i++) {
                    if (n >= 3)
                        break;
                    if (d3io_LodName(name, 0, i, buf, 0) && d3io_FindStock(buf)) {
                        d3io_AttachStock(obj, &D3_MESH(obj)->surf[n], buf);
                        n++;
                    }
                }
            }
        }
    }
}
