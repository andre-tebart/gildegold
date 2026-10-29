/* 0x42c3b0, returns the n-th of the NUL-separated fields at p. */
char *txt_Field(char *p, int n, char type)
{
    int i = 0;
    char *s = p;
    char *res;
    int k;

    if (type == 8) {
        n += 4;
        res = p;
    }
    while (i < n - 1) {
        k = 0;
        while (s[k++])
            ;
        s = s + k - 1;
        if (!s)
            goto ret_p;
        s++;
        i++;
        if (i == 4)
            res = s;
        if (i == 6 && *s)
            res = s;
    }
    if (!*s) {
        if (type == 4) {
ret_p:
            return p;
        }
        if (type == 8)
            s = res;
    }
    return s;
}
