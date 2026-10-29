/* 0x42eca0, compares two names, ignoring case if vfs_IgnoreCase is set. */
int vfs_strcmp(char *a, char *b)
{
    if (vfs_IgnoreCase)
        return stricmp(a, b);
    return strcmp(a, b);
}
