/* 0x426240, sets the directory sample bank names are opened in (sb_Path) and appends a backslash unless
   path already ends in one; a NULL path leaves it unchanged */

void sb_SetPath(char *path)
{
    if (path) {
        strcpy(sb_Path, path);
        if (path[strlen(path) - 1] != '\\')
            strcat(sb_Path, "\\");
    }
}
