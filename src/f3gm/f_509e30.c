/* 0x509e30, the sum of the amounts of the objects in the list (prototype kind 9, group 1) */

int gm_Unk509e30(void *list, unsigned char p)
{
    int s;
    char *o;

    s = 0;
    o = gm_Query(list, 1, 4, 9);
    while (o) {
        s += *(int *)(o + 0xe);
        o = (char *)gm_QueryNext();
    }
    return s;
}
