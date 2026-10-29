/* 0x4c1f90, the location track playing, -1 if none */

int snd_FindActiveTrack(void)
{
    int i;
    G16LOC *l;

    i = 0;
    l = snd_LocationTracks;
    do {
        if (l->active != 0)
            return i;
        i++;
    } while ((l++)->kinds[0] != 0);
    return -1;
}
