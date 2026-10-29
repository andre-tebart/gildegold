/* 0x5d33b0, the file list of a directory (256 B): lists the files of dirPath whose extension equals ext
   (compared case-insensitively) into records (0x210 bytes each: the name at +9, the full
   "<dirPath>/<name>" path at +0x109 with its extension cut off; the 9 bytes in front of the name are the
   caller's slot header). vfs_FindDir (dirPath, vfs_Root) gives the directory; each name is copied and
   formatted with "%s/%s" (0x62d7d8). Returns the number of entries written. */

int ai_ListFiles(char *dirPath, const char *ext, char *records)
{
    VFSDIR *dir;
    VFSENTRY *entry;
    char foundExt[0x20];
    char *dot;
    char *out;
    int i;
    int count;

    dir = vfs_FindDir(dirPath, vfs_Root);
    if (dir == 0)
        return 0;
    count = 0;
    i = 0;
    if (i < dir->nentries) {
        out = records + 9;
        do {
            entry = &dir->entries[i];
            dot = strrchr(entry->name, '.');
            if (dot != 0)
                strncpy(foundExt, dot, 0x20);
            if (_strcmpi(foundExt, ext) == 0) {
                strcpy(out, entry->name);
                sprintf(out + 0x100, "%s/%s", dirPath, entry->name);
                dot = strrchr(out, '.');
                if (dot != 0)
                    *dot = '\0';
                count++;
                out += 0x210;
            }
            i++;
        } while (i < dir->nentries);
    }
    return count;
}
