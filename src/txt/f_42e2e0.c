/* 0x42e2e0, returns the index of a loaded text file, or -1. */

int txt_FindFile(char *name)
{
    int i;

    for (i = 0; i < 0x80; i++)
        if (!stricmp(txt_Files[i].name, name) && txt_Files[i].name[0])
            return i;
    return -1;
}
