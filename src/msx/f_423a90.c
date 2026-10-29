/* 0x423a90 */

MSXTRACK *msx_Play(char *name, int once)
{
    MSXTRACK *t;

    if (!msx_Driver)
        return &msx_Dummy;
    EnterCriticalSection(&msx_Lock);
    t = msx_Find(name);
    LeaveCriticalSection(&msx_Lock);
    if (!t) {
        EnterCriticalSection(&msx_Lock);
        t = msx_Alloc();
        if (t) {
            memset(t, 0, sizeof(*t));
            strcpy(t->name, name);
        }
        LeaveCriticalSection(&msx_Lock);
    }
    t->bAmbient = 0;
    if (msx_Start(t, once) == -1)
        t = NULL;
    return t;
}
