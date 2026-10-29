/* 0x401040, strips the directory and the extension (in place) */

char *util_BaseName(char *path)
{
    char *p, *name;

    p = strrchr(path, '\\');
    name = p + 1;
    if (!p)
        name = path;
    p = strrchr(name, '/');
    if (p)
        name = p + 1;
    p = strrchr(path, '.');
    if (p)
        *p = 0;
    return name;
}
