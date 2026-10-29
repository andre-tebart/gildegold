/* 0x40d6e0, collects the objects whose name contains f->pattern */

char ocmd_FindCB(D3OBJECT *obj, OCMDFIND *f)
{
    if (strstr(obj->name, f->pattern)) {
        f->objs[f->n] = obj;
        f->n++;
    }
    return f->n < 32;
}
