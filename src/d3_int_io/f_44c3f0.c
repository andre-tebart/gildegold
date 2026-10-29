/* 0x44c3f0, loads an object group file (or reads it from f) below parent; returns the first object */

D3OBJECT *d3_LoadGroup(char *name, void *f, D3OBJECT *parent)
{
    D3OBJECT *first = 0;
    D3OBJECT *obj;
    void *in;
    int version;
    int n, i;

    if (f)
        in = f;
    else
        in = vfs_Tfopen(name, "rb", vfs_Root);
    if (in) {
        bio_rd_i(in, &version);
        if ((version & 0xffff0000) == 0x3a6c0000) {
            if (version >= 0x3a6c00ae) {
                d3_LodMissing = 0;
                bio_rd_i(in, &n);
                for (i = 0; i < n; i++) {
                    obj = rd_object(in, parent, version, 1);
                    if (first && obj && parent)
                        d3_AppendSibling(first, obj);
                    if (obj && !first)
                        first = obj;
                }
            } else
                first = rd_object(in, parent, version, 1);
            if (!f)
                vfs_fclose(in);
            if (first && parent)
                d3_AddChild(parent, first);
        } else if (!f)
            vfs_fclose(in);
    }
    return first;
}
