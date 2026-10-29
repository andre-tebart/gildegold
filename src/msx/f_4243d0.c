/* 0x4243d0, sets the directory prefix (msx_Path) that msx_Start puts before every track file name;
   unchecked strcpy into a 0x100-byte buffer, no return value */

void msx_SetPath(char *path)
{
    strcpy(msx_Path, path);
}
