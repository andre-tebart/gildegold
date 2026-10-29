/* 0x42ed40, turns slashes into backslashes. */

void vfs_ToBackslashes(char *path)
{
    int i, n;

    if (path) {
        n = strlen(path);
        for (i = 0; i < n; i++) {
            if (*path == '/')
                *path = '\\';
            path++;
        }
    }
}
