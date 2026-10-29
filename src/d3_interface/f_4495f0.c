/* 0x4495f0, callback of d3_FindObject: sets ctx->found to the first object that matches */

static char d3_FindObjectCB(D3OBJECT *obj, D3FINDCTX *ctx)
{
    char *s = (char *)obj;
    char *t;
    if (ctx->name) {
        t = (*s == '!') ? s + 1 : s;
        if (ctx->obj) {
            if (!strcmp(t, ctx->name) && ctx->obj == s)
                ctx->found = s;
        } else if (!strcmp(t, ctx->name))
            ctx->found = s;
    } else if (ctx->obj == s)
        ctx->found = s;
    return ctx->found != s;
}
