/* 0x418080, the sound slot for water type; *slot: -1 any, 0 only slot 1, 1 only slot 0 (the slot not
   taken yet); returns the slot's sound and sets *slot to it, or NULL */

extern char d3sndw_Names[6][25];  /* 0x5f38d8, the definition is below (declaration order = .data order) */
D3SOUND *d3sndw_GetSound(int type, int *slot)
{
    D3SOUND *s = NULL;

    switch (*slot) {
    case -1:
        if (!d3snd_IsPlaying(&d3sndw_Snd[0])) {
            s = &d3sndw_Snd[0];
            if (d3sndw_Snd[0].snd)
                sb_SetRestart((char *)d3sndw_Snd[0].snd, 0);
            d3sndw_Snd[0].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (d3sndw_Snd[0].snd && d3sndw_Snd[0].snd->entry) {
                sb_SetRestart((char *)d3sndw_Snd[0].snd, 1);
                strncpy(d3sndw_Snd[0].name, d3sndw_Snd[0].snd->entry->name, 0x32);
                *slot = 0;
            } else
                s = NULL;
        } else if (!strcmp(d3sndw_Snd[0].name, d3sndw_Names[type])) {
            s = &d3sndw_Snd[0];
            *slot = 0;
        } else if (!d3snd_IsPlaying(&d3sndw_Snd[1])) {
            s = &d3sndw_Snd[1];
            if (d3sndw_Snd[1].snd)
                sb_SetRestart((char *)d3sndw_Snd[1].snd, 0);
            d3sndw_Snd[1].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (d3sndw_Snd[1].snd && d3sndw_Snd[1].snd->entry) {
                sb_SetRestart((char *)d3sndw_Snd[1].snd, 1);
                strncpy(d3sndw_Snd[1].name, d3sndw_Snd[1].snd->entry->name, 0x32);
                *slot = 1;
            } else
                s = NULL;
        } else if (!strcmp(d3sndw_Snd[1].name, d3sndw_Names[type])) {
            s = &d3sndw_Snd[1];
            *slot = 1;
        }
        break;
    case 0:
        if (!d3snd_IsPlaying(&d3sndw_Snd[1])) {
            s = &d3sndw_Snd[1];
            if (d3sndw_Snd[1].snd)
                sb_SetRestart((char *)d3sndw_Snd[1].snd, 0);
            d3sndw_Snd[1].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (d3sndw_Snd[1].snd && d3sndw_Snd[1].snd->entry) {
                sb_SetRestart((char *)d3sndw_Snd[1].snd, 1);
                strncpy(d3sndw_Snd[1].name, d3sndw_Snd[1].snd->entry->name, 0x32);
                *slot = 1;
            } else
                s = NULL;
        } else if (!strcmp(d3sndw_Snd[1].name, d3sndw_Names[type])) {
            s = &d3sndw_Snd[1];
            *slot = 1;
        }
        break;
    case 1:
        if (!d3snd_IsPlaying(&d3sndw_Snd[0])) {
            s = &d3sndw_Snd[0];
            if (d3sndw_Snd[0].snd)
                sb_SetRestart((char *)d3sndw_Snd[0].snd, 0);
            d3sndw_Snd[0].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (!d3sndw_Snd[0].snd || !d3sndw_Snd[0].snd->entry) {
                s = NULL;
                *slot = 0;
            } else {
                sb_SetRestart((char *)d3sndw_Snd[0].snd, 1);
                strncpy(d3sndw_Snd[0].name, d3sndw_Snd[0].snd->entry->name, 0x32);
                *slot = 0;
            }
        } else if (!strcmp(d3sndw_Snd[0].name, d3sndw_Names[type]))
            s = &d3sndw_Snd[0];
        break;
    }
    return s;
}
