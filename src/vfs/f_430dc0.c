/* 0x430dc0, reads a line; the line end (any run of CR/LF) is skipped. */

char *vfs_fgets(char *buf, int n, VFSFILE *f)
{
    int c, i;
    char *p;

    c = vfs_fgetc(f);
    if (c == -1)
        return 0;
    p = buf;
    i = 0;
    while (c != 13 && c != 10 && c != -1 && i < n) {
        *p++ = c;
        i++;
        c = vfs_fgetc(f);
    }
    *p = 0;
    if (c != -1) {
        while (c == 13 || c == 10)
            c = vfs_fgetc(f);
    }
    return buf;
}
