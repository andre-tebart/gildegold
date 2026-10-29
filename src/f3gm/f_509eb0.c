/* 0x509eb0 */

int gm_Unk509eb0(char *c)
{
    int s;
    char *o;

    s = 0;
    o = gm_Query(*(void **)(c + 0x178), 1, 4, 9);
    while (o) {
        s += *(int *)(o + 0xe);
        o = (char *)gm_QueryNext();
    }
    return s;
}
