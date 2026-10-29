/* 0x42b720 */
int sfx_Startup(void)
{
    int ret = 0;

    if (!sfx_Initialized) {
        MSS_auto_cleanup();
        AIL_startup();
        AIL_last_error();
        sfx_Initialized = -1;
    } else
        ret = -1;
    return ret;
}
