/* 0x44e080, loads an object file and its variants (the name pair for lod -1, 1, 2) */

D3IOOBJ *d3io_LoadObject(char *a, char *b)
{
    char x[256];
    char y[256];
    D3IOOBJ *ret;
    int t;
    int i;

    d3io_LodName(a, b, 0, y, x);
    t = d3io_FindStock(x);
    if (t == 0 && (t = d3io_FindStock(y), t == 0)) {
        ret = d3io_LoadObjectFile(y, x);
        if (d3io_LodName(a, b, -1, y, x))
            d3io_LoadObjectFile(y, x);
        if (d3io_Lod.mode == 1) {
            for (i = 1; i < 3; i++) {
                if (d3io_LodName(a, b, i, y, x))
                    d3io_LoadObjectFile(y, x);
            }
            return ret;
        }
        return ret;
    }
    return (D3IOOBJ *)t;
}
