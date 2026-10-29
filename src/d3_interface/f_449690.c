/* 0x449690, finds an object by name (and/or address `c`), case-sensitive, among the objects of the default scene
   selected by `flags`; the siblings after `a` are cut off during the search */
D3OBJECT *d3_FindObject(int a, int flags, int c, char *name)
{
    D3FINDCTX ctx;
    D3OBJECT *next;

    if (!c && !name)
        return 0;
    ctx.obj = (char *)c;
    ctx.name = name;
    ctx.found = 0;
    if (a) {
        next = ((D3OBJECT *)a)->next;
        ((D3OBJECT *)a)->next = 0;
    }
    d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)a, flags | 0x200, (D3OBJECTCB)d3_FindObjectCB, &ctx);
    if (a)
        ((D3OBJECT *)a)->next = next;
    return (D3OBJECT *)ctx.found;
}
