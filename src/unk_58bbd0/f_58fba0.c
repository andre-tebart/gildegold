/* 0x58fba0, the "bk_" site finder of 0x58fc40 (148 B, a d3_ForEachObject callback found through the pointer):
   objects whose name (or name + 1) starts with "bk_" and that stand within 100.0 of ctx->pos are stored to
   ctx->object; the part of the name from the "bk_" prefix on is copied to ctx->name; returns 0 (stops the
   traversal), 1 when the object does not match */

char ai_FindBauplatz(int obj, AIBUILDSITE *ctx)
{
    float pos[4];
    char *s;
    int d;
    char c;

    if (_strnicmp((char *)obj, "bk_", 3) != 0 && _strnicmp((char *)(obj + 1), "bk_", 3) != 0)
        return 1;
    d3_PointToWorld((struct D3OBJECT *)obj, (struct D3VECTOR *)(obj + 0x4c), (struct D3VECTOR *)pos);
    if (d3_VecEqual((struct D3VECTOR *)ctx->pos, (struct D3VECTOR *)pos, 100.0f) != 0) {
        s = strstr((char *)obj, "bk_");
        if (s != 0) {
            d = ctx->name - s;
            do {
                c = *s;
                s[d] = c;
                s++;
            } while (c != '\0');
        }
        ctx->object = obj;
        return 0;
    }
    return 1;
}
