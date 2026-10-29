/* 0x500970, adds the amount n of the object prototype proto to the owner id (a building / object / character found by 0x4fea50): an existing instance gets +n (+0xe), else 0x500190 creates one; an unknown owner writes the message "gm_AddObjekt(): Parent not found. ObjProt:%i Menge:%i" (the buffer is not shown). Returns the instance or 0 */

int gm_AddObjekt(int id, int proto, int n)
{
    int b, o, c, r;
    int *list;
    char buf[512];

    if (proto != 0) {
        if (gm_FindById(&b, &o, &c, id) != 0) {
            if (b == 0) {
                if (o == 0) {
                    if (c == 0)
                        return 0;
                    list = (int *)(c + 0x178);
                } else
                    list = (int *)(o + 0x14);
            } else
                list = (int *)(b + 0x5d);
            r = (int)gm_Query(*list, 1, 0, proto);
            if (r != 0) {
                *(int *)(r + 0xe) += n;
                return r;
            }
            return gm_CreateObject(id, proto, n);
        }
        sprintf(buf, "gm_AddObjekt(): Parent not found. ObjProt:%i Menge:%i", proto, n);
    }
    return 0;
}
