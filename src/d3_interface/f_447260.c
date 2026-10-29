/* 0x447260, d3_ForEachObject callback of d3if_ShowTree (the same as d3if_Show, without the type 6 test) */

char d3if_ShowCB(D3OBJECT *obj, char show)
{
    char buf[64];
    char *p, *q;

    if (show) {
        if (obj->type == 1) {
            if (obj->name[0] == '!') {
                p = obj->name + 1;
                q = buf;
                while ((*q++ = *p++))
                    ;
                p = buf;
                q = obj->name;
                while ((*q++ = *p++))
                    ;
            }
            if (obj->type0 == 5)
                d3_LightInvalidate(0);
            obj->type = obj->type0;
            return 1;
        }
    } else if (obj->type != 1) {
        obj->type0 = obj->type;
        obj->type = 1;
    }
    return 1;
}
