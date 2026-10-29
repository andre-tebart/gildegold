/* 0x500ae0, like 0x500a40 but creates a missing instance with 0 first and removes it when the amount is exactly 0 (a negative stock is possible) */

int gm_AddNegative(int id, int proto, int n)
{
    int b, o, c, r, left;
    int *list;

    if (gm_FindById(&b, &o, &c, id) == 0)
        return 0;
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
    if (r == 0) {
        r = gm_CreateObject(id, proto, 1);
        *(int *)(r + 0xe) = 0;
    }
    left = *(int *)(r + 0xe) - n;
    *(int *)(r + 0xe) = left;
    if (left == 0)
        gm_RemoveObject(list, proto);
    return 1;
}
