/* 0x423830, opens the music and ambient drivers, the timer and the mixer thread; 0 ok, -1 on failure
   (the music driver is closed again) */

int msx_Init(int a, int b, int c)
{
    DWORD id;

    memset(&msx_Dummy, 0, sizeof(msx_Dummy));
    InitializeCriticalSection(&msx_Lock);
    if (sfx_OpenDigOutput(&msx_Driver, a, b, c) == 0 && sfx_OpenDigOutput(&msx_AmbDriver, a, b, c) == 0) {
        if (!QueryPerformanceFrequency(&msx_Freq)) {
            sfx_CloseDigOutput(msx_Driver);
            msx_Driver = 0;
            return -1;
        }
        if (!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)msx_Thread, NULL, 0, &id)) {
            sfx_CloseDigOutput(msx_Driver);
            msx_Driver = 0;
            return -1;
        }
        return 0;
    }
    return -1;
}
