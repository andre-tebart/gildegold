/* 0x4282a0 */

int evt_Preprocess(char *src, char *dst, int *lines)
{
    unsigned int i = 0;
    int nl = 0;
    int n, j;
    char *e;

    strcpy(dst, src);
    if (lines)
        lines[0] = 0;
    for (; i < strlen(dst); i++) {
        switch (dst[i]) {
        case '\n':
        case '\r':
        case 0x10:
            if (lines && dst[i] == '\n')
                lines[++nl] = i + 1;
        case '\t':
            dst[i] = ' ';
            break;
        case '/':
            if (dst[i + 1] == '*' && (e = strstr(dst + i + 1, "*/")))
                memset(dst + i, ' ', e - (dst + i) + 2);
            break;
        }
    }
    for (i = 0; i < strlen(dst); i++) {
        if (dst[i] == ' ') {
            n = 1;
            while (dst[i + n] == ' ')
                n++;
            if (lines && n > 1) {
                memmove(dst + i + 1, dst + i + n, strlen(dst + i + n) + 1);
                for (j = 0; j < nl; j++)
                    if (lines[j] >= (int)i) {
                        lines[j] = lines[j] - n + 1;
                        if (lines[j] < (int)i)
                            lines[j] = i;
                        if (lines[j] < 0)
                            lines[j] = 0;
                    }
            }
        }
    }
    lines[nl] = -1;
    return strlen(dst);
}
