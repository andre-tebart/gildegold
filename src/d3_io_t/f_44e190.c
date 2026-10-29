/* 0x44e190, the same, counting a reference on every object loaded */

D3IOOBJ *d3io_LoadObjectRef(char *a, char *b)
{
    char x[256];
    char y[256];
    D3IOOBJ *ret;
    D3IOOBJ *o;
    int t;
    int i;

    d3io_LodName(a, b, 0, y, x);
    t = d3io_FindStock(x);
    if (t == 0 && (t = d3io_FindStock(y), t == 0)) {
        ret = d3io_LoadObjectFile(y, x);
        if (ret != 0)
            ret->refs++;
        if (d3io_LodName(a, b, -1, y, x) && (o = d3io_LoadObjectFile(y, x), o != 0))
            o->refs++;
        if (d3io_Lod.mode == 1) {
            for (i = 1; i < 3; i++) {
                if (d3io_LodName(a, b, i, y, x) && (o = d3io_LoadObjectFile(y, x), o != 0))
                    o->refs++;
            }
            return ret;
        }
        return ret;
    }
    return (D3IOOBJ *)t;
}
