/* 0x449710, like d3_FindObjectCB but case-insensitive */

static char d3_FindObjectNoCaseCB(D3OBJECT *obj, D3FINDCTX *ctx)
{
    char *s = (char *)obj;
    char *t;
    if (ctx->name) {
        t = (*s == '!') ? s + 1 : s;
        if (ctx->obj) {
            if (!_strcmpi(t, ctx->name) && ctx->obj == s)
                ctx->found = s;
        } else if (!_strcmpi(t, ctx->name))
            ctx->found = s;
    } else if (ctx->obj == s)
        ctx->found = s;
    return ctx->found != s;
}
