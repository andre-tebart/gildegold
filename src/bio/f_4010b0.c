/* 0x4010b0, the next line that is neither empty nor a ';' comment, trimmed and upper-cased */

char *util_ReadLine(VFSFILE *f)
{
    int i;
    char *p;

    while (vfs_fgets(util_Line, 0x100, f)) {
        for (i = strlen(util_Line) - 1; i > -1; i--) {
            if (util_Line[i] != '\n' && util_Line[i] != '\r' && util_Line[i] != '\t' && util_Line[i] != ' ')
                break;
        }
        if (i >= 0) {
            util_Line[i + 1] = 0;
            for (p = util_Line; *p; p++) {
                if (*p != '\n' && *p != '\r' && *p != '\t' && *p != ' ')
                    break;
            }
            if (*p && *p != ';') {
                _strupr(p);
                return p;
            }
        }
    }
    return 0;
}
