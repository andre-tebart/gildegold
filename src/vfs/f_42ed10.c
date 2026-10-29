/* 0x42ed10, turns backslashes into slashes. */

void vfs_ToSlashes(char *path)
{
    int i, n;

    if (path) {
        n = strlen(path);
        for (i = 0; i < n; i++) {
            if (*path == '\\')
                *path = '/';
            path++;
        }
    }
}
