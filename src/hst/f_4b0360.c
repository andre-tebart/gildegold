/* 0x4b0360, the VERMOEGEN text command (callback 0x601b50[7], name 0x601070[7]) "-PLUS_n" / "-MINUS_n" for the
   character args[k]: gives or takes n percent of 0x509e60 (0x4c4830) */

int ai_CommandWealth(G12ARG *args, int k, char *fmt)
{
    int sign[2] = { -1, 1 };
    char name[2][6] = { "MINUS", "PLUS" };
    int i, n, v;
    float f;
    AICHAR *c;

    if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0 && *fmt == '-') {
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i < 2) {
            fmt += strlen(name[i]);
            if (*fmt == '_') {
                n = atoi(fmt + 1);
                f = n * 0.01f;
                v = (int)(gm_GetGoodsValue(c) * f);
                if (sign[i] == -1)
                    cm_SendMoveGoodsUnchecked(-1, c->netId, 0, v);
                else
                    cm_SendMoveGoodsUnchecked(c->netId, -1, 0, v);
                return 1;
            }
        }
    }
    return 0;
}
