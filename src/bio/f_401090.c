/* 0x401090, 1 if the file exists in the vfs */

int file_Exists(char *path)
{
    return vfs_FindFile(path, (int)vfs_Root, (int *)&path) != 0;
}
