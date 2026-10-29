/* 0x44df40, loads an object file (name + ".bgf") and links it into the stock list */

D3IOOBJ *d3io_LoadObjectFile(char *name, char *file)
{
    D3IOOBJ *obj = 0;
    char *p;
    void *data;
    int y, z;
    void *keep;
    char path[256];

    p = d3io_StockFile(name, ".bgf");
    if (p) {
        y = 0;
        z = 0;
        data = 0;
        strcpy(path, p);
        d3iot_LoadTexSet(file, (char **)&data, &y, &z);
        if (data == 0)
            d3iot_LoadTexSet(name, (char **)&data, &y, &z);
        keep = data;
        obj = (D3IOOBJ *)d3iot_LoadBGF(path, file, (char *)data, y, z);
        if (obj != 0) {
            if (keep != 0 && obj->data != keep)
                mem_Free(keep);
            obj->fn1e8 = (void *)d3iot_DeleteStockObject;
            obj->fn1ec = (void *)d3_TransformNormals;
            obj->fn1f0 = (void *)d3_TransformVerts;
            obj->fn1f4 = (void *)d3_GetBoxVerts;
            obj->fn1f8 = (void *)d3iot_GetRadius;
        } else {
            if (keep != 0)
                mem_Free(keep);
        }
        if (obj != 0) {
            obj->next = d3_StockList;
            d3_StockList = obj;
            obj->prev = &d3_StockHead;
            obj->next->prev = obj;
        }
    }
    return obj;
}
