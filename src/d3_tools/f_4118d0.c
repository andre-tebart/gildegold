/* 0x4118d0, d3_FindObject callback: keeps the object whose name matches (case-insensitive) and stops the search */

static char d3t_FindObjectCB(D3OBJECT *obj, void *name)
{
    if (_strcmpi(obj->name, name) == 0) {
        d3t_FoundObject = obj;
        return 0;
    }
    return 1;
}
