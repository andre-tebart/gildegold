/* 0x447190, unlinks and disposes an object */

char d3if_Delete(D3OBJECT *obj)
{
    if (obj) {
        d3_Unlink(obj);
        d3_DisposeObject(obj);
        return 1;
    }
    return 0;
}
