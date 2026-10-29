/* 0x423770 */
void msx_Thread(void)
{
    LARGE_INTEGER now;

    while (!msx_Quit) {
        if (!QueryPerformanceCounter(&now))
            msx_Exit();
        else if (now.QuadPart - msx_Last.QuadPart > msx_Freq.QuadPart / 20) {
            msx_Last = now;
            EnterCriticalSection(&msx_Lock);
            msx_Update();
            LeaveCriticalSection(&msx_Lock);
        }
        Sleep(23);
    }
    DeleteCriticalSection(&msx_Lock);
}
