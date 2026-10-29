/* 0x431510, shuts the virtual file system down: frees the directory tree, warns (err_Message) about the
   files still open when warn is set, closes them, clears the entry pool and the case-insensitivity flag;
   side effects: vfs_Root / vfs_OpenCount / vfs_EntryPool / vfs_IgnoreCase */

void vfs_Exit(char warn)
{
    MPOOLITER it;
    char msg[0x100];
    VFSFILE *f;

    if (vfs_Root) {
        vfs_FreeDir(vfs_Root);
        vfs_Root = 0;
    }
    if (vfs_OpenCount && warn) {
        sprintf(msg, "%i vfs_file(s) were left open!", vfs_OpenCount);
        err_Message(msg);
    }
    if (vfs_FilePool) {
        for (f = pool_First(vfs_FilePool, &it); f; f = pool_Next(&it))
            vfs_fclose(f);
    }
    vfs_OpenCount = 0;
    if (vfs_EntryPool)
        pool_Destroy(&vfs_EntryPool);
    vfs_IgnoreCase = 0;
}
