/* 0x424250, finds the track slot whose file name equals `name` (plain strcmp; callers hold msx_Lock);
   returns the slot or NULL */

MSXTRACK *msx_Find(char *name)
{
    int i;

    for (i = 0; i < MSX_TRACKS; i++)
        if (strcmp(msx_Tracks[i].name, name) == 0)
            return &msx_Tracks[i];
    return NULL;
}
