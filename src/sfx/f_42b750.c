/* 0x42b750 */

int sfx_Shutdown(void)
{
    int ret = 0;

    if (sfx_Initialized) {
        sfx_CloseAllStreams();
        sfx_ReleaseAllSamples();
        sfx_CloseAll();
        AIL_shutdown();
        sfx_Initialized = ret;
    } else
        ret = -1;
    return ret;
}
