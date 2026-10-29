/* 0x500a40, takes the amount n of the prototype proto from the owner id: 1 when it had enough (the instance is removed when it drops to 0 or below), else 0 */

int gm_TakeObjekt(int id, int proto, int n)
{
    int b, o, c, r, left;
    int *list;

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
        if (r != 0 && n <= *(int *)(r + 0xe)) {
            left = *(int *)(r + 0xe) - n;
            *(int *)(r + 0xe) = left;
            if (left < 1)
                gm_RemoveObject(list, proto);
            return 1;
        }
    }
    return 0;
}
