/* 0x449770, d3_FindObject with a case-insensitive name compare */

D3OBJECT *d3_FindObjectNoCase(int a, int flags, int c, char *name)
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
    d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)a, flags | 0x200, (D3OBJECTCB)d3_FindObjectNoCaseCB, &ctx);
    if (a)
        ((D3OBJECT *)a)->next = next;
    return (D3OBJECT *)ctx.found;
}
