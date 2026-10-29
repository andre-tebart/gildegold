/* 0x509060, up to three town objects (query group 2: kinds 7, 4 = 0x1d) whose owner word (+0x1c) is the id of the character c; returns the count and stores them in out */

int gm_Unk509060(char *c, int *out)
{
    int n;
    char *o;

    n = 0;
    o = gm_Query(0, 2, 7, 4, 0x1d);
    while (o && n < 3) {
        if (*(int *)(o + 0x1c) == *(int *)(c + 1)) {
            if (out)
                out[n] = (int)o;
            n++;
        }
        o = (char *)gm_QueryNext();
    }
    return n;
}
