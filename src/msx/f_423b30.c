/* 0x423b30, opens and rewinds (or queues) the track's stream, looping unless once; -1 when the stream fails */

int msx_Start(MSXTRACK *t, int once)
{
    char path[256];
    MSXTRACK *q;

    if (msx_Driver && msx_AmbDriver) {
        EnterCriticalSection(&msx_Lock);
        if (msx_DelayPeak)
            sprintf(msx_Msg, "Reporting last delaypeak: %d", AIL_get_timer_highest_delay());
        else
            msx_DelayPeak = AIL_get_timer_highest_delay();
        if (!t->playing) {
            strcpy(path, msx_Path);
            strcat(path, t->name);
            if (t->stream == 0) {
                if (t->bAmbient ? sfx_OpenStream(msx_AmbDriver, path, &t->stream)
                               : sfx_OpenStream(msx_Driver, path, &t->stream) == -1) {
                    LeaveCriticalSection(&msx_Lock);
                    return -1;
                }
            }
            if (t->length == 0) {
                sfx_GetStreamLength(t->stream, &t->length);
                t->volume = 127;
            }
            if ((!t->bAmbient && t->length - t->pos < 10000) || t->pos == 0) {
                t->pos = 0;
                t->curvol = t->volume;
                t->fademode = 0;
                sfx_SetStreamVolume(t->stream, 0);
                sfx_StartStream(t->stream);
                sfx_PauseStream(t->stream, 1);
                sfx_SetStreamPos(t->stream, 0);
                sfx_SetStreamVolume(t->stream, t->volume);
                t->fadepos = 0;
            } else {
                sfx_PauseStream(t->stream, 1);
                sfx_SetStreamVolume(t->stream, 0);
                sfx_SetStreamPos(t->stream, t->pos);
                if (!t->bAmbient) {
                    t->curvol = 0;
                    t->fadevol = 0;
                    t->fademode = 1;
                    t->fadepos = t->pos;
                }
            }
            if (once)
                sfx_SetStreamLoopCount(t->stream, 0);
            else
                sfx_SetStreamLoopCount(t->stream, 1);
            q = msx_GetPlaying();
            if (!t->bAmbient && q) {
                sprintf(msx_Msg, "Queuing Track %s after Track %s !", t->name, q->name);
                q->next = t;
                t->queued = 1;
                msx_SetFade(q, 2);
            } else {
                sprintf(msx_Msg, "Starting Track %s !", t->name);
                t->queued = 0;
                sfx_PauseStream(t->stream, 0);
                t->playing = 1;
            }
        } else if (t->fademode == 2)
            msx_SetFade(t, 1);
        LeaveCriticalSection(&msx_Lock);
    }
    return 0;
}
