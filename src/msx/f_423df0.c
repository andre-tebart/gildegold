/* 0x423df0, stops a playing track: a fade request starts a fade-out (fademode 2, the update thread
   closes the stream once it has faded), otherwise the stream is paused and closed at once. No return
   value; takes msx_Lock and clears the track's stream/playing */

void msx_Stop(MSXTRACK *t, int fade)
{
    if (msx_Driver) {
        EnterCriticalSection(&msx_Lock);
        if (t->playing) {
            if (fade)
                msx_SetFade(t, 2);
            else {
                sfx_PauseStream(t->stream, 1);
                sfx_CloseStream(t->stream);
                t->stream = 0;
                t->playing = 0;
            }
        }
        LeaveCriticalSection(&msx_Lock);
    }
}
