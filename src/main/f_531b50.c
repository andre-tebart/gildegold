/* 0x531b50, creates the single instance mutex "Die Gilde" (the handle stored in *mutex); returns 1 when it already existed (ERROR_ALREADY_EXISTS), 0 when new, -1 on failure */

unsigned main_CreateMutex(void **mutex)
{
    HANDLE h;

    h = CreateMutexA(0, 1, "Die Gilde");
    if (h != 0) {
        *mutex = h;
        return GetLastError() == 0xb7;
    }
    return 0xffffffff;
}
