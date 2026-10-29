/* 0x4471b0, hides (show = 0: type saved in +0x21c and set to 1) or shows an object again */

char d3if_Show(D3OBJECT *obj, char show)
{
    char buf[64];
    char *p, *q;

    if (obj) {
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
                if (obj->type0 == 5 || obj->type0 == 6)
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
    return 0;
}
