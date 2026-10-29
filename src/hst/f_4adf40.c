/* 0x4adf40, copies src to dst up to the second '-', '%' becomes " %" after a non-blank; *n1 = the characters
   read, *n2 = the length of the rest */
void ai_SplitTokenFormat(char *src, char *dst, int *n1, int *n2)
{
    char *p;
    char *d;
    char prev;
    int n;

    d = dst;
    n = 0;
    prev = ' ';
    for (p = src; *p != 0 && n < 2; p++) {
        if (*p == '-') {
            n++;
        } else {
            if (*p == '%') {
                if (prev == ' ') {
                    *d = '%';
                } else {
                    *d = ' ';
                    prev = *p;
                    d++;
                    *d = prev;
                }
            } else {
                prev = *p;
                *d = *p;
            }
            d++;
        }
    }
    *n1 = p - src;
    *n2 = strlen(p);
}
