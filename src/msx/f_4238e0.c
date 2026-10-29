/* 0x4238e0, asks the mixer thread to stop, then closes the music and ambient drivers */

void msx_Exit(void)
{
    msx_Quit = 1;
    if (msx_Driver) {
        sfx_CloseAllStreams();
        sfx_CloseDigOutput(msx_Driver);
        msx_Driver = 0;
    }
    if (msx_AmbDriver) {
        sfx_CloseDigOutput(msx_AmbDriver);
        msx_AmbDriver = 0;
    }
}
