/* 0x4bc4b0, d3_ForEachObject callback: collects the objects named f->name (at most 32) */

int rd_CollectNamed(char *obj, G15FIND *f)
{
    if (strcmp(obj, f->name) == 0)
        f->objs[f->n++] = obj;
    return f->n < 0x20;
}
