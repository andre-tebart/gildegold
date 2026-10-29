/* 0x4244b0, stops an ambient track at once (no fade; msx_Stop with fade 0): asserts that the track is
   ambient, then pauses and closes its stream under msx_Lock. Does nothing without the ambient driver,
   no return value */

void msx_StopAmbient(MSXTRACK *hTrack)
{
    if (msx_AmbDriver) {
#line 741
        assert(hTrack->bAmbient);
        EnterCriticalSection(&msx_Lock);
        msx_Stop(hTrack, 0);
        LeaveCriticalSection(&msx_Lock);
    }
}
