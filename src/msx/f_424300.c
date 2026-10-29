/* 0x424300, picks a free track slot for a new track: first an idle one with no position yet, then any
   idle one; returns the slot or NULL when all MSX_TRACKS are busy. Callers hold msx_Lock */

MSXTRACK *msx_Alloc(void)
{
    int i;

    for (i = 0; i < MSX_TRACKS; i++)
        if (!msx_Tracks[i].playing && !msx_Tracks[i].stream && !msx_Tracks[i].pos)
            return &msx_Tracks[i];
    for (i = 0; i < MSX_TRACKS; i++)
        if (!msx_Tracks[i].playing && !msx_Tracks[i].stream)
            return &msx_Tracks[i];
    return NULL;
}
