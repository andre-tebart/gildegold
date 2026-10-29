/* 0x431490, initializes the virtual file system: resets the open count, stores the caller's
   case-insensitivity flag in vfs_IgnoreCase and builds the directory tree from root (vfs_AddDir), logging
   root and the current directory; side effects: vfs_OpenCount / vfs_Root / vfs_EntryPool. Returns 1 when the
   root was added, 0 otherwise */

char vfs_Init(char *root, char unk)
{
    char cwd[0x100];
    char msg[0x100];

    vfs_OpenCount = 0;
    vfs_IgnoreCase = unk;
    vfs_Root = vfs_AddDir(root, 0);
    _getcwd(cwd, 0x100);
    sprintf(msg, "vfs_init: root -> \"%s\" relative to \"%s\"", root, cwd);
    vfs_EntryPool = 0;
    return vfs_Root != 0;
}
