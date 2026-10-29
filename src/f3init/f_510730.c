/* 0x510730, d3_ForEachObject callback of init_LoadSiteScene: keeps the object o when its name contains the
   group key of the list (ctx->key, the site name); returns 0 once 0x200 objects are collected */


char init_CollectSiteObject(D3OBJECT *o, void *ctx)
{
    INIT_SITELIST *list = (INIT_SITELIST *)ctx;

    if (strstr(o->name, list->key) != 0) {
        list->obj[list->n] = o;
        list->n++;
    }
    return list->n < 0x200;
}
