/* 0x4243f0, plays the named sound once on the ambient driver: reuses the slot of that name or takes a
   free one, closes a stream still open in it and starts the track at `vol`. Returns the track, NULL
   when no slot is free or the stream fails, &msx_Dummy when no ambient driver is open; takes msx_Lock */

MSXTRACK *msx_PlayAmbient(char *name, int vol)
{
    MSXTRACK *t;

    if (!msx_AmbDriver)
        return &msx_Dummy;
    EnterCriticalSection(&msx_Lock);
    t = msx_Find(name);
    if (!t) {
        t = msx_Alloc();
        if (!t)
            goto fail;
        memset(t, 0, sizeof(*t));
        strcpy(t->name, name);
    }
    t->bAmbient = 1;
    if (t->stream) {
        sfx_CloseStream(t->stream);
        t->stream = 0;
    }
    if (msx_Start(t, 1) == 0) {
        msx_SetTrackVolume(t, vol);
        LeaveCriticalSection(&msx_Lock);
        return t;
    }
fail:
    LeaveCriticalSection(&msx_Lock);
    return NULL;
}
