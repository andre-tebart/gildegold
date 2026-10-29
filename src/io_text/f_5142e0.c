/* 0x5142e0, the display name of the town called name (the player's town, "General"/"Stadt" of the game INI): the first text of the town's "_STADTAUSWAHL_<NAME>_INFO" group of the text table (txt_FindKey finds its number), else the given name itself; copied to out. Returns 1. */
int io_TownDisplayName(char *name, char *out)
{
    char key[0x100];
    int idx;

    sprintf(key, "_STADTAUSWAHL_%s_INFO+0", name);
    _strupr(key);
    idx = txt_FindKey(key);
    if (idx != -1) {
        strcpy(out, txt_Ptrs[idx]);
        return 1;
    }
    strcpy(out, name);
    return 1;
}
