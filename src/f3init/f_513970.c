/* 0x513970, splits the comma separated list s into at most n numbers (atoi of each part) in out; returns the count */


int init_ParseNumberList(char *s, int *out, int n)
{
    int done, cnt;
    char *c;
    char buf[0x100];

    cnt = 0;
    done = 0;
    c = strchr(s, ',');
    if (0 < n) {
        do {
            memset(buf, 0, 0x100);
            strncpy(buf, s, c - s);
            out[cnt] = atoi(buf);
            cnt++;
            if (done)
                return cnt;
            s = c + 1;
            c = strchr(s, ',');
            if (c == 0) {
                c = strchr(s, 0);
                done = 1;
            }
        } while (cnt < n);
    }
    return cnt;
}
